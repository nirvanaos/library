/*
* Nirvana C runtime library.
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
#include <Nirvana/c_heap_dbg.h>

using namespace Nirvana;

#ifndef NIRVANA_NO_BAD_ALLOC
#define NIRVANA_BAD_ALLOC_TRY try {
#define NIRVANA_BAD_ALLOC_CATCH } catch (const CORBA::NO_MEMORY&) { throw std::bad_alloc (); }
#else
#define NIRVANA_BAD_ALLOC_TRY
#define NIRVANA_BAD_ALLOC_CATCH
#endif

void* operator new (size_t cb)
{
	NIRVANA_BAD_ALLOC_TRY
		return c_alloc <HeapBlockHdrType> (alignof (std::max_align_t), cb, 0);
	NIRVANA_BAD_ALLOC_CATCH
}

void* operator new[] (size_t cb)
{
	NIRVANA_BAD_ALLOC_TRY
		return c_alloc <HeapBlockHdrType> (alignof (std::max_align_t), cb, 0);
	NIRVANA_BAD_ALLOC_CATCH
}

void* operator new (size_t cb, const std::nothrow_t&) noexcept
{
	return c_alloc <HeapBlockHdrType> (alignof (std::max_align_t), cb, Memory::EXACTLY);
}

void* operator new[] (size_t cb, const std::nothrow_t&) noexcept
{
	return c_alloc <HeapBlockHdrType> (alignof (std::max_align_t), cb, Memory::EXACTLY);
}

void operator delete (void* p) noexcept
{
	c_free <HeapBlockHdrType> (p);
}

void operator delete[] (void* p) noexcept
{
	c_free <HeapBlockHdrType> (p);
}

#ifdef NIRVANA_C14

void operator delete (void* p, size_t cb) noexcept
{
	c_free <HeapBlockHdrType> (p);
}

void operator delete[] (void* p, size_t cb) noexcept
{
	c_free <HeapBlockHdrType> (p);
}

#ifdef NIRVANA_C17

void* operator new (size_t cb, std::align_val_t al)
{
	assert (cb >= (size_t)al);
	NIRVANA_BAD_ALLOC_TRY
		return Nirvana::the_memory->allocate (nullptr, cb, 0);
	NIRVANA_BAD_ALLOC_CATCH
}

void* operator new[] (size_t cb, std::align_val_t al)
{
	NIRVANA_BAD_ALLOC_TRY
		return c_alloc <HeapBlockHdrType> (static_cast <size_t> (al), cb, 0);
	NIRVANA_BAD_ALLOC_CATCH
}

void* operator new (size_t cb, std::align_val_t al, const std::nothrow_t&) noexcept
{
	assert (cb >= (size_t)al);
	return Nirvana::the_memory->allocate (nullptr, cb, Memory::EXACTLY);
}

void* operator new[] (size_t cb, std::align_val_t al, const std::nothrow_t&) noexcept
{
	return c_alloc <HeapBlockHdrType> (static_cast <size_t> (al), cb, Memory::EXACTLY);
}

void operator delete (void* p, std::align_val_t al) noexcept
{
	NIRVANA_UNREACHABLE_CODE ();
}

void operator delete[] (void* p, std::align_val_t al) noexcept
{
	c_free <HeapBlockHdrType> (p);
}

void operator delete (void* p, size_t cb, std::align_val_t al) noexcept
{
	Nirvana::the_memory->release (p, cb);
}

void operator delete[] (void* p, size_t cb, std::align_val_t al) noexcept
{
	c_free <HeapBlockHdrType> (p);
}

#endif // NIRVANA_C17
#endif // NIRVANA_C14
