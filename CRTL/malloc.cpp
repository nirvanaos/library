//#include <stdlib.h>
#include <Nirvana/Nirvana.h>

namespace Nirvana {

struct HeapBlockHdr
{
	HeapBlockHdr (size_t cb) :
		size (cb)
	{}

	size_t size;
};

}

using namespace Nirvana;

extern "C" {

void* malloc (size_t size)
{
	void* mem = g_memory->allocate (nullptr, size + sizeof (HeapBlockHdr), Memory::EXACTLY);
	if (mem) {
		HeapBlockHdr* block = new (mem) HeapBlockHdr (size);
		return block + 1;
	}
	return nullptr;
}

void* _malloc_base (size_t size)
{
	return malloc (size);
}

void* _malloc_dbg (size_t size, int blockType, const char* filename, int linenumber)
{
	return malloc (size);
}

void* calloc (size_t num, size_t size)
{
	void* mem = g_memory->allocate (nullptr, num * size + sizeof (HeapBlockHdr), Memory::EXACTLY | Memory::ZERO_INIT);
	if (mem) {
		HeapBlockHdr* block = new (mem) HeapBlockHdr (size);
		return block + 1;
	}
	return nullptr;
}

void* _calloc_base (size_t num, size_t size)
{
	return calloc (num, size);
}

void* _calloc_dbg (size_t num, size_t size, int blockType, const char* filename, int linenumber)
{
	return calloc (num, size);
}

void _free_base (void* p)
{
	if (p) {
		HeapBlockHdr* block = (HeapBlockHdr*)p - 1;
		g_memory->release (block, block->size);
	}
}

void free (void* p)
{
	if (p) {
		HeapBlockHdr* block = (HeapBlockHdr*)p - 1;
		g_memory->release (block, block->size);
	}
}

void _free_dbg (void* p, int _BlockUse)
{
	free (p);
}

}
