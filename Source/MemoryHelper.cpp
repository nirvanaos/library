#include <Nirvana/MemoryHelper.h>
#include <Nirvana/real_copy.h>
#include <exception>
#include <algorithm>

namespace Nirvana {

void* MemoryHelper::reserve (void* p, size_t& allocated, size_t data_size, size_t capacity) const
{
	assert (p || (!data_size && !allocated));
	assert (capacity >= data_size);
	if (allocated < capacity) {
		try {
			size_t cur_capacity = allocated;
			if (cur_capacity) {
				size_t au = mem_->query (p, Memory::ALLOCATION_UNIT);
				assert (!(cur_capacity % au));
				cur_capacity = round_up (cur_capacity, au);
				capacity = round_up (capacity, au);
				size_t append = capacity - cur_capacity;
				if (mem_->allocate ((uint8_t*)p + cur_capacity, append, Memory::RESERVED | Memory::EXACTLY)) {
					allocated = capacity;
					return p;
				}
			}
			void* pnew = mem_->allocate (0, capacity, Memory::RESERVED);
			size_t au = mem_->query (pnew, Memory::ALLOCATION_UNIT);
			capacity = round_up (capacity, au);
			if (data_size)
				mem_->copy (pnew, p, data_size, cur_capacity ? Memory::RELEASE : 0);
			else if (cur_capacity)
				mem_->release (p, cur_capacity);
			p = pnew;
			allocated = capacity;
		} catch (const CORBA::NO_MEMORY&) {
			throw std::bad_alloc ();
		}
	}
	return p;
}

void MemoryHelper::shrink_to_fit (void* p, size_t& allocated, size_t data_size)
{
	assert (p && allocated && data_size <= allocated);
	size_t au = mem_->query (p, Memory::ALLOCATION_UNIT);
	size_t reserve = round_up (data_size, au);
	if (allocated > reserve)
		mem_->release ((uint8_t*)p + reserve, allocated - reserve);
	allocated = reserve;
}

void* MemoryHelper::assign (void* p, size_t& allocated, size_t old_size, size_t new_size, const void* src_ptr) const
{
	assert (p || (!old_size && !allocated));
	try {
		if (allocated >= new_size) {
			if (old_size > new_size)
				mem_->decommit ((uint8_t*)p + new_size, old_size - new_size);
			if (new_size) {
				if (src_ptr)
					mem_->copy (p, (void*)src_ptr, new_size, 0);
				else if (new_size > old_size)
					mem_->commit ((uint8_t*)p + old_size, new_size - old_size);
			}
		} else {
			void* pnew = src_ptr ?
				mem_->copy (0, (void*)src_ptr, new_size, Memory::ALLOCATE)
				:
				mem_->allocate (0, new_size, 0);
			if (allocated)
				mem_->release (p, allocated);
			size_t au = mem_->query (pnew, Memory::ALLOCATION_UNIT);
			p = pnew;
			allocated = round_up (new_size, au);
		}
		return p;
	} catch (const CORBA::NO_MEMORY&) {
		throw std::bad_alloc ();
	}
}

void MemoryHelper::erase (void* p, size_t data_size, size_t offset, size_t count) const
{
	assert (p && offset + count <= data_size);
	uint8_t* dst = (uint8_t*)p + offset;
	uint8_t* src = dst + count;
	uint8_t* end = (uint8_t*)p + data_size;
	if (src != end)
		mem_->copy (dst, src, end - src, Memory::DECOMMIT);
	else
		mem_->decommit (dst, end - dst);
}

void* MemoryHelper::replace (void* p, size_t& allocated, size_t data_size, size_t offset, size_t old_size, size_t new_size, const void* src_ptr) const
{
	assert (p || (!data_size && !allocated));
	assert (offset + old_size <= data_size);
	if ((old_size == 0 && new_size == 0) || (!src_ptr && old_size == new_size))
		return p;
	if (offset == 0 && old_size == data_size)
		return assign (p, allocated, data_size, new_size, src_ptr);

	try {
		size_t capacity = allocated;
		size_t size = data_size + new_size - old_size;
		size_t release_size = 0;
		void* pnew = p;
		if (size > capacity) {
			if (capacity) {
				size_t au = mem_->query (p, Memory::ALLOCATION_UNIT);
				assert (!(capacity % au));
				capacity = round_up (capacity, au);
				size_t new_capacity = round_up (size, au);
				size_t append = new_capacity - capacity;
				if (mem_->allocate ((uint8_t*)p + capacity, append, Memory::RESERVED | Memory::EXACTLY))
					capacity = new_capacity;
			}
			if (size > capacity) {
				pnew = mem_->allocate (0, size, Memory::RESERVED | Memory::EXACTLY);
				release_size = capacity;
				size_t au = mem_->query (p, Memory::ALLOCATION_UNIT);
				capacity = round_up (size, au);
				if (offset)
					mem_->copy (pnew, p, offset, 0);
			}
			allocated = capacity;
		}
		uint8_t* dst = (uint8_t*)pnew + offset;

		if (old_size != new_size) {

			// Copy tail
			size_t tail = offset + old_size;
			if (tail < data_size)
				mem_->copy (dst + new_size, (uint8_t*)p + tail, data_size - tail, Memory::DECOMMIT);

			// Decommit
			if (old_size > new_size && pnew == p) {
				size_t decommit = old_size - new_size;
				mem_->decommit ((uint8_t*)p + data_size - decommit, decommit);
			}
		}

		if (src_ptr)
			mem_->copy (dst, (void*)src_ptr, new_size, 0);
		else
			mem_->commit (dst, new_size);

		if (release_size)
			mem_->release (p, release_size);

		return pnew;

	} catch (const CORBA::NO_MEMORY&) {
		throw std::bad_alloc ();
	}
}

}

