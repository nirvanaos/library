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
#include "../../pch/pch.h"

namespace Nirvana {

void* MemoryHelper::reserve_internal (void* p, size_t& allocated, size_t data_size, size_t capacity)
{
	assert (p || (!data_size && !allocated));
	assert (capacity >= data_size);
	assert (allocated < capacity);
	size_t cur_capacity = allocated;
	if (cur_capacity) {
		if (expand_internal (p, cur_capacity, capacity, Memory::RESERVED)) {
			allocated = capacity;
			return p;
		}
	}
	void* pnew = memory ()->allocate (0, capacity, Memory::RESERVED);
	if (data_size)
		memory ()->copy (pnew, p, data_size, cur_capacity ? Memory::SRC_RELEASE : 0);
	else if (cur_capacity)
		memory ()->release (p, cur_capacity);
	p = pnew;
	allocated = capacity;
	return p;
}

void MemoryHelper::shrink_to_fit_internal (void* p, size_t& allocated, size_t data_size)
{
	assert (p && allocated && data_size <= allocated);
	size_t reserve = 0;
	if (data_size) {
		size_t au = memory ()->query (p, Memory::QueryParam::ALLOCATION_UNIT);
		reserve = round_up (data_size, au);
	}
	if (allocated > reserve)
		memory ()->release ((uint8_t*)p + reserve, allocated - reserve);
	allocated = reserve;
}

void* MemoryHelper::assign_internal (void* p, size_t& allocated, size_t old_size, size_t new_size, const void* src_ptr)
{
	assert (p || (!old_size && !allocated));
	if (allocated >= new_size) {
		if (old_size > new_size)
			decommit_internal ((uint8_t*)p + new_size, old_size - new_size);
		if (new_size) {
			if (src_ptr)
				memory ()->copy (p, (void*)src_ptr, new_size, 0);
			else if (new_size > old_size)
				commit_internal ((uint8_t*)p + old_size, new_size - old_size);
		}
	} else {
		void* pnew = src_ptr ?
			memory ()->copy (nullptr, (void*)src_ptr, new_size, Memory::DST_ALLOCATE)
			:
			memory ()->allocate (nullptr, new_size, 0);
		if (allocated)
			memory ()->release (p, allocated);
		p = pnew;
		allocated = new_size;
	}
	return p;
}

void MemoryHelper::erase_internal (void* p, size_t data_size, size_t offset, size_t count)
{
	assert (p && offset + count <= data_size);
	uint8_t* dst = (uint8_t*)p + offset;
	uint8_t* src = dst + count;
	uint8_t* end = (uint8_t*)p + data_size;
	if (src != end) {
		size_t cb = end - src;
		memory ()->copy (dst, src, cb, Memory::SRC_DECOMMIT);
	} else
		decommit_internal (dst, end - dst);
}

void* MemoryHelper::replace_internal (void* p, size_t& allocated, size_t data_size, size_t offset, size_t old_size, size_t new_size, const void* src_ptr)
{
	assert (p || (!data_size && !allocated));
	assert (offset + old_size <= data_size);
	if ((old_size == 0 && new_size == 0) || (!src_ptr && old_size == new_size))
		return p;
	if (offset == 0 && old_size == data_size)
		return assign_internal (p, allocated, data_size, new_size, src_ptr);

	size_t capacity = allocated;
	size_t size = data_size + new_size - old_size;
	size_t release_size = 0;
	void* pnew = p;
	if (size > capacity) {
		if (capacity) {
			if (expand_internal (p, capacity, size, Memory::RESERVED))
				capacity = size;
		}
		if (size > capacity) {
			pnew = memory ()->allocate (nullptr, size, Memory::RESERVED);
			release_size = capacity;
			capacity = size;
			if (offset)
				memory ()->copy (pnew, p, offset, 0);
		}
		allocated = capacity;
	}
	uint8_t* dst = (uint8_t*)pnew + offset;

	if (old_size != new_size) {

		// Copy tail
		size_t tail = offset + old_size;
		if (tail < data_size) {
			size_t cb = data_size - tail;
			memory ()->copy (dst + new_size, (uint8_t*)p + tail, cb, Memory::SRC_DECOMMIT);
		}

		// Decommit
		if (old_size > new_size && pnew == p) {
			size_t decommit = old_size - new_size;
			decommit_internal ((uint8_t*)p + data_size - decommit, decommit);
		}
	}

	if (src_ptr)
		memory ()->copy (dst, (void*)src_ptr, new_size, 0);
	else
		commit_internal (dst, new_size);

	if (release_size)
		memory ()->release (p, release_size);

	return pnew;
}

bool MemoryHelper::expand_internal (void* p, size_t cur_size, size_t& new_size, unsigned flags) noexcept
{
	assert (cur_size && new_size > cur_size);
	assert (cur_size % (size_t)memory ()->query (p, Memory::QueryParam::ALLOCATION_UNIT) == 0);
	// Pointer must be aligned to clp2 (new_size)
	if ((uintptr_t)p % clp2 (new_size))
		return false;
	size_t append = new_size - cur_size;
	if (memory ()->allocate ((uint8_t*)p + cur_size, append, (uint16_t)flags | Memory::EXACTLY) != nullptr) {
		new_size = cur_size + append;
		return true;
	}
	return false;
}

bool MemoryHelper::on_the_page (void* p, size_t size) noexcept
{
	uintptr_t begin = (uintptr_t)p;
	uintptr_t end = begin + size;
	uintptr_t page_begin = round_down (begin, MIN_COMMIT_PAGE);
	uintptr_t page_end = page_begin + MIN_COMMIT_PAGE;
	return begin > page_begin && end <= page_end;
}

}

