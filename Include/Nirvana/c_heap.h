/// \file
/// \brief Implementation of the C heap functions: malloc, free etc.
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
#ifndef NIRVANA_C_HEAP_H_
#define NIRVANA_C_HEAP_H_
#pragma once

#include "Nirvana.h"
#include <limits>

namespace Nirvana {

class HeapBlockHdr
{
public:
	HeapBlockHdr (void* begin, size_t cb) noexcept :
		begin_ (begin),
		size_ (cb)
	{}

	static const size_t TRAILER_SIZE = 0;

	static HeapBlockHdr* hdr_from_ptr (void* p) noexcept
	{
		return (HeapBlockHdr*)p - 1;
	}

	void* begin () const noexcept
	{
		return begin_;
	}

	void* end () const noexcept
	{
		return (char*)begin_ + size_;
	}

	size_t allocated_size () const noexcept
	{
		return size_;
	}

	void resize (size_t new_size) noexcept
	{
		size_ = new_size;
	}

	void check () const noexcept
	{
		assert ((const char*)begin_ + size_ >= (const char*)(this + 1));
	}

protected:
	void* begin_; // Allocated memory begin
	size_t size_; // Allocated memory size
};

template <class Hdr, typename ... Args> inline
void* c_alloc (size_t alignment, size_t size, unsigned short flags, Args&& ... args)
{
	size_t padding = alignment > sizeof (Hdr) ? alignment - sizeof (Hdr) : 0;
	size_t cb = size + padding + sizeof (Hdr) + Hdr::TRAILER_SIZE;
	void* mem = the_memory->allocate (nullptr, cb, flags);
	if (mem) {
		Hdr* block = new ((char*)mem + padding) Hdr (mem, cb, std::forward <Args> (args)...);
		return block + 1;
	}
	return nullptr;
}

template <class Hdr, typename ... Args> inline
void* c_malloc (size_t alignment, size_t size, Args&& ... args)
{
	return c_alloc <Hdr> (alignment, size, Memory::EXACTLY, std::forward <Args> (args)...);
}

template <class Hdr, typename ... Args> inline
void* c_calloc (size_t count, size_t element_size, Args&& ... args)
{
	if (std::numeric_limits <size_t>::max () / element_size < count)
		return nullptr;
	return c_alloc <Hdr> (alignof (max_align_t), element_size * count,
		Memory::EXACTLY | Memory::ZERO_INIT, std::forward <Args> (args)...);
}

template <class Hdr> inline
void c_free (void* p)
{
	if (p) {
		Hdr* block = Hdr::hdr_from_ptr (p);
		block->check ();
		the_memory->release (block->begin (), block->allocated_size ());
	}
}

template <class Hdr> inline
size_t c_usable_size (void* p)
{
	if (p) {
		Hdr* block = Hdr::hdr_from_ptr (p);
		block->check ();
		return (const char*)block->end () - (const char*)p - Hdr::TRAILER_SIZE;
	} else
		return 0;
}

template <class Hdr, typename ... Args> inline
void* c_realloc (void* p, size_t size, Args&& ... args)
{
	if (!p)
		return c_malloc <Hdr> (alignof (max_align_t), size, std::forward <Args> (args)...);

	if (!size) {
		c_free <Hdr> (p);
		return nullptr;
	}

	Hdr* block = Hdr::hdr_from_ptr (p);
	block->check ();

	char* end = (char*)block->end ();
	size_t cur_size = end - Hdr::TRAILER_SIZE - (char*)p;

	if (size < cur_size) {
		// Shrink
		size_t rel = cur_size - size;
		size_t au = the_memory->query (p, Memory::QueryParam::ALLOCATION_UNIT);
		rel = round_down (rel, au);
		if (rel) {
			the_memory->release (end - rel, rel);
			block->resize (block->allocated_size () - rel, std::forward <Args> (args)...);
		}
	} else if (size > cur_size) {
		// Try expand
		size_t exp = size - cur_size;
		if (the_memory->allocate (end, exp, Memory::EXACTLY))
			block->resize (block->allocated_size () + exp, std::forward <Args> (args)...);
		else {
			// Reallocate with the same alignment
			size_t padding = (char*)p - (char*)block->begin () - sizeof (Hdr);
			size_t cb = padding + sizeof (Hdr) + size + Hdr::TRAILER_SIZE;
			char* new_begin = (char*)the_memory->allocate (nullptr, cb, Memory::RESERVED | Memory::EXACTLY);
			if (!new_begin)
				return nullptr;
			size_t old_block_size = block->allocated_size ();
			try {
				the_memory->commit (new_begin + old_block_size, cb - old_block_size);
				the_memory->copy (new_begin, block->begin (), old_block_size, Memory::SRC_RELEASE);
			} catch (...) {
				the_memory->release (new_begin, cb);
				return nullptr;
			}
			Hdr* new_block = new ((char*)new_begin + padding) Hdr (new_begin, cb, std::forward <Args> (args)...);
			p = new_block + 1;
		}
	}

	return p;
}

}

#endif
