#include <Nirvana/MemoryHelper.h>

namespace Nirvana {

void* MemoryHelper::reserve (void* p, size_t data_size, size_t cur_capacity, size_t& new_capacity) const
{
	if (cur_capacity < new_capacity) {
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
		}
	}
}

void* MemoryHelper::assign (void* dst, size_t& capacity, const void* data_ptr, size_t data_size) const
{
	if (capacity >= data_size) {
		mem_->copy (dst, (void*)data_ptr, data_size, 0);
	} else {
		void* pnew = mem_->copy (0, (void*)data_ptr, data_size, Memory::ALLOCATE);
		if (dst)
			mem_->release (dst, capacity);
		dst = pnew;
		size_t au = mem_->query (pnew, Memory::ALLOCATION_UNIT);
		capacity = round_up (data_size, au);
	}
}

}

