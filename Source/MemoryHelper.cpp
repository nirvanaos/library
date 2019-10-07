#include <Nirvana/MemoryHelper.h>
#include <Nirvana/real_copy.h>
#include <exception>
#include <algorithm>

namespace Nirvana {

void* MemoryHelper::reserve (void* p, size_t data_size, size_t cur_capacity, size_t& new_capacity) const
{
	assert (data_size <= cur_capacity);
	if (cur_capacity < new_capacity) {
		try {
			if (p) {
				size_t au = mem_->query (p, Memory::ALLOCATION_UNIT);
				assert (!(cur_capacity % au));
				cur_capacity = round_up (cur_capacity, au);
				new_capacity = round_up (new_capacity, au);
				size_t append = new_capacity - cur_capacity;
				if (mem_->allocate ((uint8_t*)p + cur_capacity, append, Memory::RESERVED | Memory::EXACTLY))
					return p;
			}
			void* pnew = mem_->allocate (0, new_capacity, Memory::RESERVED);
			size_t aunew = mem_->query (pnew, Memory::ALLOCATION_UNIT);
			new_capacity = round_up (new_capacity, aunew);
			if (p) {
				if (data_size)
					mem_->copy (pnew, p, data_size, Memory::RELEASE);
				mem_->release (p, cur_capacity);
			}
			p = pnew;
		} catch (const CORBA::NO_MEMORY&) {
			throw std::bad_alloc ();
		}
	}
	return p;
}

void* MemoryHelper::assign (void* p, size_t& capacity, size_t size, const void* src_ptr, size_t src_size) const
{
	assert (size <= capacity);
	try {
		if (capacity >= src_size) {
			mem_->copy (p, (void*)src_ptr, src_size, 0);
			// Aggressive, may affect performance?
			if (size > src_size)
				mem_->decommit ((uint8_t*)p + src_size, size - src_size);
		} else {
			void* pnew = mem_->copy (0, (void*)src_ptr, src_size, Memory::ALLOCATE);
			if (p)
				mem_->release (p, capacity);
			p = pnew;
			size_t au = mem_->query (pnew, Memory::ALLOCATION_UNIT);
			capacity = round_up (src_size, au);
		}
		return p;
	} catch (const CORBA::NO_MEMORY&) {
		throw std::bad_alloc ();
	}
}

void* MemoryHelper::commit (void* p, size_t& capacity, size_t old_size, size_t new_size) const
{
	assert (old_size <= capacity);
	try {
		if (capacity >= new_size) {
			if (old_size < new_size)
				mem_->commit ((uint8_t*)p + old_size, new_size - old_size);
			else if (old_size > new_size)
				mem_->decommit ((uint8_t*)p + new_size, old_size - new_size);
		} else {
			void* pnew = mem_->allocate (0, new_size, 0);
			if (p)
				mem_->release (p, capacity);
			p = pnew;
			size_t au = mem_->query (pnew, Memory::ALLOCATION_UNIT);
			capacity = round_up (new_size, au);
		}
		return p;
	} catch (const CORBA::NO_MEMORY&) {
		throw std::bad_alloc ();
	}
}

void MemoryHelper::shrink_to_fit (void* p, size_t& capacity, size_t size)
{
	assert (size <= capacity);
	size_t au = mem_->query (p, Memory::ALLOCATION_UNIT);
	size_t reserve = round_up (size, au);
	if (capacity > reserve)
		mem_->release ((uint8_t*)p + reserve, capacity - reserve);
	if (size < reserve)
		mem_->decommit ((uint8_t*)p + size, reserve - size);
	capacity = reserve;
}

void MemoryHelper::erase (void* p, size_t size, size_t off, size_t count) const
{
	assert (off + count <= size);
	uint8_t* dst = (uint8_t*)p + off;
	uint8_t* src = dst + count;
	uint8_t* end = (uint8_t*)p + size;
	if (src != end)
		mem_->copy (dst, src, end - src, Memory::DECOMMIT);
	else
		mem_->decommit (dst, end - dst);
}

void* MemoryHelper::insert (void* p, size_t& capacity, size_t size, size_t where, const void* src_ptr, size_t src_size) const
{
	try {
		assert (size <= capacity);
		assert (where <= size);
		size_t new_size = size + src_size;
		if (new_size > capacity) {
			size_t new_capacity = new_size;
			void* pnew = reserve (p, size, capacity, new_capacity);
			if (pnew != p && (p <= src_ptr && src_ptr < (uint8_t*)p + size))
				src_ptr = (uint8_t*)pnew + ((uint8_t*)src_ptr - (uint8_t*)p);
			p = pnew;
			capacity = new_capacity;
		}
		uint8_t* dst = (uint8_t*)p + where;
		if (where < size)
			// Copy tail
			mem_->copy (dst + src_size, dst, size - where, Memory::DECOMMIT);
		if (src_ptr)
			mem_->copy (dst, (void*)src_ptr, src_size, 0);
		else
			mem_->commit (dst, src_size);
		return p;
	} catch (const CORBA::NO_MEMORY&) {
		throw std::bad_alloc ();
	}
}

}

