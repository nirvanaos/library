#include <Nirvana/c_heap_dbg.h>

using namespace Nirvana;

extern "C" {

void* malloc (size_t size)
{
	return c_malloc <HeapBlockHdrType> (size);
}

void* calloc (size_t num, size_t size)
{
	return c_calloc <HeapBlockHdrType> (num, size);
}

void* realloc (void* p, size_t size)
{
	return c_realloc <HeapBlockHdrType> (p, size);
}

void free (void* p)
{
	c_free <HeapBlockHdrType> (p);
}

}
