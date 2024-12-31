/// \file Nirvana::Memory interface mock implementation.
/*
* Nirvana mock library.
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

#include <CORBA/Server.h>
#include <Nirvana/Memory_s.h>
#include <Nirvana/real_copy.h>
#include <Nirvana/bitutils.h>
#include <malloc.h>
#include <map>
#include <mutex>
#include <type_traits>
#include "export.h"

namespace Nirvana {
namespace Test {

class Memory :
	public CORBA::servant_traits <Nirvana::Memory>::ServantStatic <Memory>
{
	static const size_t ALLOC_UNIT = 16;

public:
	// Memory::

	static void* allocate (void* dst, size_t& size, unsigned flags)
	{
		uint8_t* pdst = round_down ((uint8_t*)dst, ALLOC_UNIT);
		uint8_t* pend = round_up (flags & Memory::EXACTLY ?
			(uint8_t*)dst + size : pdst + size, ALLOC_UNIT);
		uint8_t* p = blocks ().allocate (pdst, pend - pdst, flags);
		if (p) {
			if (flags & Nirvana::Memory::ZERO_INIT)
				memset (p, 0, size);
			size = pend - pdst;
		}
		return p;
	}

	static void release (void* p, size_t size)
	{
		if (p && size) {
			uint8_t* pdst = round_down ((uint8_t*)p, ALLOC_UNIT);
			blocks ().release (pdst, round_up ((uint8_t*)p + size, ALLOC_UNIT) - pdst);
		}
	}

	static void commit (void* ptr, size_t size)
	{
		if (size)
			blocks ().check_allocated ((uint8_t*)ptr, size);
	}

	static void decommit (void* ptr, size_t size)
	{
		if (size)
			blocks ().check_allocated ((uint8_t*)ptr, size);
	}

	static void* copy (void* dst, void* src, size_t& size, unsigned flags)
	{
		if (size) {
			const uint8_t* src_end = (const uint8_t*)src + size;
			if (!dst || flags & Nirvana::Memory::DST_ALLOCATE)
				dst = allocate (dst, size, flags & ~Nirvana::Memory::ZERO_INIT);
			real_move ((const uint8_t*)src, src_end, (uint8_t*)dst);
			if ((flags & Nirvana::Memory::SRC_RELEASE) == Nirvana::Memory::SRC_RELEASE)
				release (src, src_end - (const uint8_t*)src);
		}
		return dst;
	}

	static bool is_private (const void* p, size_t size)
	{
		return true;
	}

	static intptr_t query (const void* p, Nirvana::Memory::QueryParam q)
	{
		switch (q) {
		case Nirvana::Memory::QueryParam::ALLOCATION_UNIT:
			return ALLOC_UNIT;

		case Nirvana::Memory::QueryParam::GRANULARITY:
		case Nirvana::Memory::QueryParam::PROTECTION_UNIT:
		case Nirvana::Memory::QueryParam::SHARING_ASSOCIATIVITY:
			return 4096;
		}
		return 0;
	}

	static Nirvana::Memory::_ref_type create_heap (unsigned)
	{
		throw_NO_IMPLEMENT ();
	}

	// For tests

	static size_t bytes_cnt ()
	{
		return blocks ().bytes_cnt ();
	}

private:
	static size_t alignment (size_t size)
	{
		size_t al = clp2 (size);
		if (al < 8)
			al = 8;
		return al;
	}

	static void* al_malloc (size_t size)
	{
		return _mm_malloc (size, alignment (size));
//		return _aligned_malloc (size, alignment (size));
	}
	
	static void al_free (void* p)
	{
		_mm_free (p);
//		_aligned_free (p);
	}

	NIRVANA_NORETURN static void bad_heap ()
	{
		throw CORBA::BAD_PARAM ();
	}

	template <typename T>
	class Allocator : public std::allocator <T>
	{
	public:
		static void deallocate (T* p, size_t cnt)
		{
			free (p);
		}

		static T* allocate (size_t cnt)
		{
			return (T*)malloc (cnt * sizeof (T));
		}

		template <class U>
		operator const Allocator <U>& () const noexcept
		{
			return *reinterpret_cast <const Allocator <U>*> (this);
		}

		template <class U> struct rebind
		{
			typedef Allocator <U> other;
		};

	};

	class Holes : public std::map <size_t, size_t, std::less <size_t>, Allocator <std::pair <const size_t, size_t> > > // begin -> end
	{
	public:
		bool allocate (size_t b, size_t e)
		{
			assert (b < e);
			iterator f = upper_bound (b);
			if (f != begin ()) {
				--f;
				if (f->first <= b && e <= f->second) {
					size_t hole_end = f->second;
					if (f->first == b)
						erase (f);
					else
						f->second = b;
					if (hole_end > e)
						insert (value_type (e, hole_end));
					return true;
				}
			}
			return false;
		}

		void release (size_t b, size_t e)
		{
			iterator next, prev;
			check_allocated (b, e, prev, next);
			if (prev != end () && prev->second == b)
				prev->second = e;
			else
				prev = insert (value_type (b, e)).first;
			if (next != end () && next->first == e) {
				prev->second = next->second;
				erase (next);
			}
		}

		void check_allocated (size_t b, size_t e)
		{
			if (!empty ()) {
				iterator next, prev;
				check_allocated (b, e, prev, next);
			}
		}

		size_t bytes_cnt () const
		{
			size_t cb = 0;
			for (auto p = begin (); p != end (); ++p)
				cb += p->second - p->first;
			return cb;
		}

	private:
		void check_allocated (size_t b, size_t e, iterator& prev, iterator& next)
		{
			next = upper_bound (b);
			if (next != end ()) {
				if (next->first < e)
					bad_heap ();
			}
			prev = next;
			if (prev != begin ()) {
				--prev;
				if (prev->second > b)
					bad_heap ();
			}
		}
	};

	struct Block
	{
		Block (size_t s) :
			size (s)
		{}

		bool allocate (size_t b, size_t e)
		{
			return holes.allocate (b, e);
		}

		bool release (size_t b, size_t e)
		{
			if (b == 0 && holes.empty () && e == size)
				return true;
			holes.release (b, e);
			return (holes.size () == 1) && (holes.begin ()->first == 0) && (holes.begin ()->second == size);
		}

		void check_allocated (size_t off, size_t size)
		{
			holes.check_allocated (off, size);
		}

		size_t bytes_cnt () const
		{
			return size - holes.bytes_cnt ();
		}

		size_t size;
		Holes holes;
	};

	typedef std::map <uint8_t*, Block, std::less <uint8_t*>, Allocator <std::pair <uint8_t* const, Block> > > BlockMap;

	class Blocks : private BlockMap
	{
	public:
		bool empty () const
		{
			return BlockMap::empty ();
		}

		uint8_t* allocate (uint8_t* dst, size_t size, unsigned flags)
		{
			uint8_t* ret = nullptr;
			if (dst) {
				std::lock_guard <std::mutex> lock (mutex_);
				iterator f = find_block (dst, size);
				if (f != end ()) {
					size_t b = dst - f->first;
					size_t e = dst + size - f->first;
					if (f->second.allocate (b, e))
						ret = dst;
				}
				if (flags & Nirvana::Memory::EXACTLY)
					return ret;
			}
			ret = (uint8_t*)al_malloc (size);
			if (!ret) {
				if (flags & Nirvana::Memory::EXACTLY)
					return nullptr;
				else
					throw CORBA::NO_MEMORY ();
			}
			std::lock_guard <std::mutex> lock (mutex_);
			insert (value_type (ret, size));
			return ret;
		}

		void release (uint8_t* dst, size_t size)
		{
			std::lock_guard <std::mutex> lock (mutex_);
			iterator f = find_block (dst, size);
			if (f != end ()) {
				size_t b = dst - f->first;
				size_t e = dst + size - f->first;
				if (f->second.release (b, e)) {
					al_free (f->first);
					erase (f);
				}
			} else
				bad_heap ();
		}

		void check_allocated (uint8_t* dst, size_t size)
		{
			std::lock_guard <std::mutex> lock (mutex_);
			iterator f = find_block (dst, size);
			if (f != end ())
				f->second.check_allocated (dst - f->first, size);
			else
				bad_heap ();
		}

		size_t bytes_cnt () const
		{
			size_t cb = 0;
			for (auto p = begin (); p != end (); ++p)
				cb += p->second.bytes_cnt ();
			return cb;
		}

	private:
		iterator find_block (uint8_t* dst, size_t size)
		{
			iterator f = upper_bound (dst);
			if (f != begin ()) {
				--f;
				if (f->first <= dst && dst + size <= f->first + f->second.size)
					return f;
			}
			return end ();
		}
	
		std::mutex mutex_;
	};

	static Blocks& blocks ()
	{
		std::call_once (blocks_init_, init_blocks);
		return *(Blocks*)&blocks_;
	}

	static void init_blocks ()
	{
		new (&blocks_) Blocks ();
	}

private:
	typedef std::aligned_storage <sizeof (Blocks), alignof (Blocks)>::type BlocksStorage;
	static std::once_flag blocks_init_;
	static BlocksStorage blocks_;
};

std::once_flag Memory::blocks_init_;
Memory::BlocksStorage Memory::blocks_;

extern NIRVANA_MOCK_EXPORT size_t allocated_bytes ()
{
	return Memory::bytes_cnt ();
}

NIRVANA_MOCK_EXPORT CORBA::Internal::Interface* mock_memory = NIRVANA_STATIC_BRIDGE (Nirvana::Memory, Memory);

}

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <Memory> the_memory = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, NIRVANA_STATIC_BRIDGE (Memory, Test::Memory) };

}
