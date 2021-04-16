// Nirvana project
// Tests
// g_memory object.

#include <CORBA/Server.h>
#include <malloc.h>
#include <memory.h>
#include <Nirvana/Memory_s.h>
#include <Nirvana/real_copy.h>
#include <Nirvana/bitutils.h>
#include <map>
#include <mutex>

namespace Nirvana {
namespace Test {

using namespace std;

class Memory :
	public ::CORBA::Nirvana::ServantStatic <Memory, Nirvana::Memory>
{
	static const size_t ALLOC_UNIT = 16;

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

	class Holes : public map <size_t, size_t> // begin -> end
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

	class Blocks : private map <uint8_t*, Block>
	{
	public:
		bool empty () const
		{
			return map <uint8_t*, Block>::empty ();
		}

		uint8_t* allocate (uint8_t* dst, size_t size, UWord flags)
		{
			uint8_t* ret = nullptr;
			if (dst) {
				lock_guard <mutex> lock (mytex_);
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
			ret = (uint8_t*)_aligned_malloc (size, alignment (size));
			if (!ret) {
				if (flags & Nirvana::Memory::EXACTLY)
					return nullptr;
				else
					throw CORBA::NO_MEMORY ();
			}
			lock_guard <mutex> lock (mytex_);
			insert (value_type (ret, size));
			return ret;
		}

		void release (uint8_t* dst, size_t size)
		{
			lock_guard <mutex> lock (mytex_);
			iterator f = find_block (dst, size);
			if (f != end ()) {
				size_t b = dst - f->first;
				size_t e = dst + size - f->first;
				if (f->second.release (b, e)) {
					_aligned_free (f->first);
					erase (f);
				}
			} else
				bad_heap ();
		}

		void check_allocated (uint8_t* dst, size_t size)
		{
			lock_guard <mutex> lock (mytex_);
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
	
		mutex mytex_;
	};

	static Blocks& blocks ()
	{
		static Blocks s_blocks;
		return s_blocks;
	}

public:

	// Memory::
	static void* allocate (void* dst, size_t size, UWord flags)
	{
		uint8_t* pdst = round_down ((uint8_t*)dst, ALLOC_UNIT);
		uint8_t* p = blocks ().allocate (pdst, round_up ((uint8_t*)dst + size, ALLOC_UNIT) - pdst, flags);
		p += pdst - (uint8_t*)dst;
		if (flags & Nirvana::Memory::ZERO_INIT)
			memset (p, 0, size);
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

	static void* copy (void* dst, void* src, size_t size, UWord flags)
	{
		if (size) {
			if (!dst || flags & Nirvana::Memory::ALLOCATE)
				dst = allocate (dst, size, flags & ~Nirvana::Memory::ZERO_INIT);
			real_move ((uint8_t*)src, (uint8_t*)src + size, (uint8_t*)dst);
		}
		return dst;
	}

	static bool is_readable (const void* p, size_t size)
	{
		return true;
	}

	static bool is_writable (const void* p, size_t size)
	{
		return true;
	}

	static bool is_private (const void* p, size_t size)
	{
		return true;
	}

	static bool is_copy (const void* p1, const void* p2, size_t size)
	{
		return p1 == p2;
	}

	static intptr_t query (const void* p, MemQuery q)
	{
		switch (q) {
		case MemQuery::ALLOCATION_UNIT:
			return ALLOC_UNIT;

		case MemQuery::GRANULARITY:
		case MemQuery::PROTECTION_UNIT:
		case MemQuery::SHARING_ASSOCIATIVITY:
			return 4096;
		}
		return 0;
	}

	static bool bytes_cnt ()
	{
		return blocks ().empty ();
	}
};

size_t allocated_bytes ()
{
	return Memory::bytes_cnt ();
}

}

extern const ImportInterfaceT <Memory> g_memory = { OLF_IMPORT_INTERFACE, nullptr, nullptr, NIRVANA_STATIC_BRIDGE (Memory, Test::Memory) };

}
