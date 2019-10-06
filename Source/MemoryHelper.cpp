#include <Nirvana/MemoryHelper.h>
#include <exception>
#include <algorithm>

namespace Nirvana {

void* MemoryHelper::reserve (void* p, size_t data_size, size_t cur_capacity, size_t& new_capacity) const
{
	assert (p);
	if (cur_capacity < new_capacity) {
		try {
			size_t au = mem_->query (p, Memory::ALLOCATION_UNIT);
			assert (!(cur_capacity % au));
			cur_capacity = round_up (cur_capacity, au);
			new_capacity = round_up (new_capacity, au);
			size_t append = new_capacity - cur_capacity;
			if (!mem_->allocate ((uint8_t*)p + cur_capacity, append, Memory::RESERVED | Memory::EXACTLY)) {
				void* pnew = mem_->allocate (0, new_capacity, Memory::RESERVED);
				size_t aunew = mem_->query (pnew, Memory::ALLOCATION_UNIT);
				new_capacity = round_up (new_capacity, aunew);
				if (data_size) {
					mem_->copy (pnew, p, data_size, Memory::RELEASE);
					data_size = round_up (data_size, au);
				}
				if (cur_capacity > data_size)
					mem_->release ((uint8_t*)p + data_size, cur_capacity - data_size);
				p = pnew;
			}
		} catch (const CORBA::NO_MEMORY&) {
			throw std::bad_alloc ();
		}
	}
	return p;
}

void* MemoryHelper::reserve (size_t& capacity) const
{
	try {
		void* p = mem_->allocate (0, capacity, Memory::RESERVED);
		size_t au = mem_->query (p, Memory::ALLOCATION_UNIT);
		capacity = round_up (capacity, au);
		return p;
	} catch (const CORBA::NO_MEMORY&) {
		throw std::bad_alloc ();
	}
}

void* MemoryHelper::assign (void* p, size_t& capacity, size_t size, const void* src_ptr, size_t src_size) const
{
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
	size_t au = mem_->query (p, Memory::ALLOCATION_UNIT);
	size_t reserve = round_up (size, au);
	if (capacity > reserve)
		mem_->release ((uint8_t*)p + reserve, capacity - reserve);
	if (size < reserve)
		mem_->decommit ((uint8_t*)p + size, reserve - size);
	capacity = reserve;
}

}

