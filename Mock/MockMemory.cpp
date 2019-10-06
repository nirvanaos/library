// Nirvana project
// Tests
// MockMemory class

#include <Mock/MockMemory.h>
#include <malloc.h>
#include <memory.h>
#include <Nirvana/Memory_s.h>
#include <Nirvana/real_copy.h>
#include <Nirvana/bitutils.h>

namespace Nirvana {
namespace Test {

class MockMemory :
	public ::CORBA::Nirvana::ServantStatic <MockMemory, Memory>
{
	static const size_t ALLOCATION_UNIT = 16;

public:
	static size_t alignment (size_t size)
	{
		size_t al = clp2 (size);
		if (al < 8)
			al = 8;
		return al;
	}

	// Memory::
	static void* allocate (void* dst, size_t size, long flags)
	{
		if (dst && (flags & Memory::EXACTLY))
			return nullptr;
		size = round_up (size, ALLOCATION_UNIT);
		void* ret = _aligned_malloc (size, alignment (size));
		if (flags & Memory::ZERO_INIT)
			memset (ret, 0, size);
		return ret;
	}

	static void release (void* p, size_t size)
	{
		_aligned_free (p);
	}

	static void commit (void* ptr, size_t size)
	{}

	static void decommit (void* ptr, size_t size)
	{}

	static void* copy (void* dst, void* src, size_t size, long flags)
	{
		if (!dst)
			dst = allocate (nullptr, size, 0);
		real_copy ((int*)src, (int*)src + size / sizeof (int), (int*)dst);
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
}
