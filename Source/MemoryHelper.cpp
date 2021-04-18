/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include <Nirvana/MemoryHelper.h>
#include <Nirvana/real_copy.h>
#include <CORBA/exceptions.h>
#include <exception>
#include <algorithm>

namespace Nirvana {

void* MemoryHelper::reserve (void* p, size_t& allocated, size_t data_size, size_t capacity)
{
	assert (p || (!data_size && !allocated));
	assert (capacity >= data_size);
	if (allocated < capacity) {
		size_t cur_capacity = allocated;
		if (cur_capacity) {
			size_t append = capacity - cur_capacity;
			if (expand ((uint8_t*)p + cur_capacity, append, Memory::RESERVED)) {
				allocated = capacity;
				return p;
			}
		}
		try {
			void* pnew = memory ()->allocate (0, capacity, Memory::RESERVED);
			size_t au = memory ()->query (pnew, Memory::Query::ALLOCATION_UNIT);
			capacity = round_up (capacity, au);
			if (data_size)
				memory ()->copy (pnew, p, data_size, cur_capacity ? Memory::RELEASE : 0);
			else if (cur_capacity)
				memory ()->release (p, cur_capacity);
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
	size_t au = memory ()->query (p, Memory::Query::ALLOCATION_UNIT);
	size_t reserve = round_up (data_size, au);
	if (allocated > reserve)
		memory ()->release ((uint8_t*)p + reserve, allocated - reserve);
	allocated = reserve;
}

void* MemoryHelper::assign (void* p, size_t& allocated, size_t old_size, size_t new_size, const void* src_ptr)
{
	assert (p || (!old_size && !allocated));
	try {
		if (allocated >= new_size) {
			if (old_size > new_size)
				memory ()->decommit ((uint8_t*)p + new_size, old_size - new_size);
			if (new_size) {
				if (src_ptr)
					memory ()->copy (p, (void*)src_ptr, new_size, 0);
				else if (new_size > old_size)
					memory ()->commit ((uint8_t*)p + old_size, new_size - old_size);
			}
		} else {
			void* pnew = src_ptr ?
				memory ()->copy (0, (void*)src_ptr, new_size, Memory::ALLOCATE)
				:
				memory ()->allocate (0, new_size, 0);
			if (allocated)
				memory ()->release (p, allocated);
			size_t au = memory ()->query (pnew, Memory::Query::ALLOCATION_UNIT);
			p = pnew;
			allocated = round_up (new_size, au);
		}
		return p;
	} catch (const CORBA::NO_MEMORY&) {
		throw std::bad_alloc ();
	}
}

void MemoryHelper::erase (void* p, size_t data_size, size_t offset, size_t count)
{
	assert (p && offset + count <= data_size);
	uint8_t* dst = (uint8_t*)p + offset;
	uint8_t* src = dst + count;
	uint8_t* end = (uint8_t*)p + data_size;
	if (src != end)
		memory ()->copy (dst, src, end - src, Memory::DECOMMIT);
	else
		memory ()->decommit (dst, end - dst);
}

void* MemoryHelper::replace (void* p, size_t& allocated, size_t data_size, size_t offset, size_t old_size, size_t new_size, const void* src_ptr)
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
				if (expand ((uint8_t*)p + capacity, size - capacity, Memory::RESERVED)) {
					size_t au = memory ()->query (p, Memory::Query::ALLOCATION_UNIT);
					assert (!(capacity % au));
					size_t new_capacity = round_up (size, au);
					capacity = new_capacity;
				}
			}
			if (size > capacity) {
				pnew = memory ()->allocate (0, size, Memory::RESERVED);
				release_size = capacity;
				size_t au = memory ()->query (pnew, Memory::Query::ALLOCATION_UNIT);
				capacity = round_up (size, au);
				if (offset)
					memory ()->copy (pnew, p, offset, 0);
			}
			allocated = capacity;
		}
		uint8_t* dst = (uint8_t*)pnew + offset;

		if (old_size != new_size) {

			// Copy tail
			size_t tail = offset + old_size;
			if (tail < data_size)
				memory ()->copy (dst + new_size, (uint8_t*)p + tail, data_size - tail, Memory::DECOMMIT);

			// Decommit
			if (old_size > new_size && pnew == p) {
				size_t decommit = old_size - new_size;
				memory ()->decommit ((uint8_t*)p + data_size - decommit, decommit);
			}
		}

		if (src_ptr)
			memory ()->copy (dst, (void*)src_ptr, new_size, 0);
		else
			memory ()->commit (dst, new_size);

		if (release_size)
			memory ()->release (p, release_size);

		return pnew;

	} catch (const CORBA::NO_MEMORY&) {
		throw std::bad_alloc ();
	}
}

bool MemoryHelper::expand (void* cur_end, size_t append, unsigned flags) NIRVANA_NOEXCEPT
{
	void* heap_end = (void*)memory ()->query ((uint8_t*)cur_end - 1, Memory::Query::ALLOCATION_SPACE_END);
	if (cur_end != heap_end)
		return memory ()->allocate (cur_end, append, (uint16_t)flags | Memory::EXACTLY) != nullptr;
	return false;
}

}

