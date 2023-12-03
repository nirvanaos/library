#include <Nirvana/c_heap_dbg.h>

using namespace Nirvana;

void* operator new (size_t cb)
{
	return c_malloc <HeapBlockHdrType> (cb);
}

void operator delete (void* p) noexcept
{
	c_free <HeapBlockHdrType> (p);
}

void operator delete (void* p, size_t cb) noexcept
{
	if (p) {
		HeapBlockHdrType* block = HeapBlockHdrType::hdr_from_ptr (p);
		block->check ();
		assert (block->size () == cb); // TODO: Improve diagnostic
		g_memory->release (block, block->size () + sizeof (HeapBlockHdrType) + HeapBlockHdrType::TRAILER_SIZE);
	}
}
