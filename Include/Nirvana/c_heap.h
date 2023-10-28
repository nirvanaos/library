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
	HeapBlockHdr (size_t cb) noexcept :
	size_ (cb)
	{}

	static const size_t TRAILER_SIZE = 0;

	static HeapBlockHdr* hdr_from_ptr (void* p) noexcept
	{
		return (HeapBlockHdr*)p - 1;
	}

	size_t size () const noexcept
	{
		return size_;
	}

	void resize (size_t new_size) noexcept
	{
		size_ = new_size;
	}

	void check () const noexcept
	{}

protected:
	size_t size_; // Memory block size without the header.
};

template <class Hdr, typename ... Args> inline
void* c_alloc (size_t size, unsigned short flags, Args ... args)
{
	size_t cb = size + sizeof (Hdr) + Hdr::TRAILER_SIZE;
	void* mem = g_memory->allocate (nullptr, cb, flags);
	if (mem) {
		Hdr* block = new (mem) Hdr (size, std::forward <Args> (args)...);
		return block + 1;
	}
	return nullptr;
}

template <class Hdr, typename ... Args> inline
void* c_malloc (size_t size, Args ... args)
{
	return c_alloc <Hdr> (size, Memory::EXACTLY, std::forward <Args> (args)...);
}

template <class Hdr, typename ... Args> inline
void* c_calloc (size_t count, size_t element_size, Args ... args)
{
	if (std::numeric_limits <size_t>::max () / element_size < count)
		return nullptr;
	return c_alloc <Hdr> (element_size * count, Memory::EXACTLY | Memory::ZERO_INIT, std::forward <Args> (args)...);
}

template <class Hdr> inline
void c_free (void* p)
{
	if (p) {
		Hdr* block = Hdr::hdr_from_ptr (p);
		block->check ();
		g_memory->release (block, block->size () + sizeof (Hdr) + Hdr::TRAILER_SIZE);
	}
}

template <class Hdr, typename ... Args> inline
void* c_realloc (void* p, size_t size, Args ... args)
{
	if (!p)
		return c_malloc <Hdr> (size, std::forward <Args> (args)...);
	if (!size) {
		c_free <Hdr> (p);
		return nullptr;
	}
	Hdr* block = Hdr::hdr_from_ptr (p);
	block->check ();
	size_t cur_size = block->size ();
	if (size < cur_size) {
		// Shrink
		g_memory->release ((uint8_t*)p + size, cur_size - size);
		block->resize (size, std::forward <Args> (args)...);
	} else if (size > cur_size) {
		size_t cb = size - cur_size;
		if (g_memory->allocate ((uint8_t*)p + cur_size, cb, Memory::EXACTLY)) {
			// Expanded
			block->resize (size, std::forward <Args> (args)...);
		} else {
			cb = size + sizeof (Hdr) + Hdr::TRAILER_SIZE;
			Hdr* new_block = (Hdr*)g_memory->allocate (nullptr, cb, Memory::RESERVED | Memory::EXACTLY);
			if (!new_block)
				return nullptr;
			size_t old_block_size = cur_size + sizeof (Hdr) + Hdr::TRAILER_SIZE;
			try {
				g_memory->commit ((uint8_t*)new_block + old_block_size, size - cur_size);
				g_memory->copy (new_block, block, old_block_size, Memory::SRC_RELEASE);
			} catch (...) {
				g_memory->release (new_block, size + sizeof (HeapBlockHdr));
				return nullptr;
			}
			new_block->resize (size, std::forward <Args> (args)...);
			p = new_block + 1;
		}
	}

	return p;
}

}

#endif
