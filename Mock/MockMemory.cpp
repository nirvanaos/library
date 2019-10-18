// Nirvana project
// Tests
// MockMemory class

#include <Nirvana/Nirvana.h>
#include <Mock/MockMemory.h>
#include <malloc.h>
#include <memory.h>
#include <Nirvana/Memory_s.h>
#include <Nirvana/real_copy.h>
#include <Nirvana/bitutils.h>
#include <map>

namespace Nirvana {
namespace Test {

class MockMemory :
	public ::CORBA::Nirvana::ServantStatic <MockMemory, Memory>
{
	static const size_t ALLOCATION_UNIT = 16;

	static size_t alignment (size_t size)
	{
		size_t al = clp2 (size);
		if (al < 8)
			al = 8;
		return al;
	}

	NIRVANA_NORETURN static void bad_heap ()
	{
		throw CORBA::BAD_PARAM ();
	}

	class Holes : public std::map <size_t, size_t> // begin -> end
	{
	public:
		bool allocate (size_t off, size_t size)
		{
			size_t e = off + size;
			iterator f = upper_bound (off);
			if (f != begin ()) {
				--f;
				if (f->first <= off && f->second <= e) {
					size_t hole_end = f->second;
					if (f->first == off)
						erase (f);
					else
						f->second = off;
					if (hole_end > e)
						emplace (e, hole_end);
					return true;
				}
			}
			return false;
		}

		void release (size_t off, size_t size)
		{
			size_t e = off + size;
			iterator next, prev;
			check_allocated (off, e, prev, next);
			if (prev != end () && prev->second == off)
				prev->second = e;
			else
				prev = emplace (off, e).first;
			if (next != end () && next->first == e) {
				prev->second = next->second;
				erase (next);
			}
		}

		void check_allocated (size_t off, size_t size)
		{
			iterator next, prev;
			check_allocated (off, off + size, prev, next);
		}

	private:
		void check_allocated (size_t off, size_t e, iterator& prev, iterator& next)
		{
			next = upper_bound (off);
			if (next != end ()) {
				if (next->first < e)
					bad_heap ();
			}
			prev = next;
			if (prev != begin ()) {
				--prev;
				if (prev->second > off)
					bad_heap ();
			}
		}
	};

	struct Block
	{
		Block (size_t s) :
			size (s)
		{}

		bool allocate (size_t off, size_t size)
		{
			return holes.allocate (off, size);
		}

		bool release (size_t off, size_t s)
		{
			if (off = 0 && holes.empty () && s == size)
				return true;
			holes.release (off, s);
			return (holes.size () == 1) && (holes.begin ()->first == 0) && (holes.begin ()->second == size);
		}

		void check_allocated (size_t off, size_t size)
		{
			holes.check_allocated (off, size);
		}

		size_t size;
		Holes holes;
	};

	class Blocks : public std::map <uint8_t*, Block>
	{
	public:
		uint8_t* allocate (uint8_t* dst, size_t size, long flags)
		{
			size = round_up (size, ALLOCATION_UNIT);
			uint8_t* ret = nullptr;
			if (dst) {
				iterator f = find_block (dst, size);
				if (f != end () && f->second.allocate (dst - f->first, size))
					ret = dst;
				if (flags & Memory::EXACTLY)
					return ret;
			}
			ret = (uint8_t*)_aligned_malloc (size, alignment (size));
			if (!ret) {
				if (flags & Memory::EXACTLY)
					return nullptr;
				else
					throw CORBA::NO_MEMORY ();
			}
			emplace (ret, size);
			return ret;
		}

		void release (uint8_t* dst, size_t size)
		{
			iterator f = find_block (dst, size);
			if (f != end ()) {
				if (f->second.release (dst - f->first, size)) {
					_aligned_free (f->first);
					erase (f);
				}
			} else
				bad_heap ();
		}

		void check_allocated (uint8_t* dst, size_t size)
		{
			iterator f = find_block (dst, size);
			if (f != end ())
				f->second.check_allocated (dst - f->first, size);
			else
				bad_heap ();
		}

	private:
		iterator find_block (uint8_t* dst, size_t size)
		{
			iterator f = upper_bound (dst);
			if (f != begin ()) {
				--f;
				if (f->first <= dst && f->first + f->second.size <= dst + size)
					return f;
			}
			return end ();
		}
	};

	static Blocks& blocks ()
	{
		static Blocks s_blocks;
		return s_blocks;
	}

public:

	// Memory::
	static void* allocate (void* dst, size_t size, long flags)
	{
		uint8_t* pdst = round_down ((uint8_t*)dst, ALLOCATION_UNIT);
		uint8_t* p = blocks ().allocate (pdst, round_up ((uint8_t*)dst + size, ALLOCATION_UNIT) - pdst, flags);
		if (flags & Memory::ZERO_INIT)
			memset (p, 0, size);
		return p;
	}

	static void release (void* p, size_t size)
	{
		uint8_t* pdst = round_down ((uint8_t*)p, ALLOCATION_UNIT);
		blocks ().release (pdst, round_up ((uint8_t*)p + size, ALLOCATION_UNIT) - pdst);
	}

	static void commit (void* ptr, size_t size)
	{
		blocks ().check_allocated ((uint8_t*)ptr, size);
	}

	static void decommit (void* ptr, size_t size)
	{
		blocks ().check_allocated ((uint8_t*)ptr, size);
	}

	static void* copy (void* dst, void* src, size_t size, long flags)
	{
		if (!dst || flags & Memory::ALLOCATE)
			dst = allocate (dst, size, flags & ~Memory::ZERO_INIT);
		real_move ((uint8_t*)src, (uint8_t*)src + size, (uint8_t*)dst);
		return dst;
	}

	static bool is_readable (const void* p, size_t size)
	{
		blocks ().check_allocated ((uint8_t*)p, size);
		return true;
	}

	static bool is_writable (const void* p, size_t size)
	{
		blocks ().check_allocated ((uint8_t*)p, size);
		return true;
	}

	static bool is_private (const void* p, size_t size)
	{
		blocks ().check_allocated ((uint8_t*)p, size);
		return true;
	}

	static bool is_copy (const void* p1, const void* p2, size_t size)
	{
		return p1 == p2;
	}

	static intptr_t query (const void* p, Memory::QueryParam q)
	{
		switch (q) {
		case Memory::ALLOCATION_UNIT:
			return ALLOCATION_UNIT;

		case Memory::GRANULARITY:
		case Memory::PROTECTION_UNIT:
		case Memory::SHARING_ASSOCIATIVITY:
			return 4096;
		}
		return 0;
	}
};

Memory_ptr mock_memory ()
{
	return MockMemory::_get_ptr ();
}
}

::CORBA::Nirvana::Bridge <Memory>* const g_default_heap = STATIC_BRIDGE (Test::MockMemory, Memory);

}
