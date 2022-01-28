#include <Nirvana/c_heap.h>

using namespace Nirvana;

extern "C" {

void* malloc (size_t size)
{
	return Nirvana::c_malloc <HeapBlockHdr> (size);
}

void* calloc (size_t num, size_t size)
{
	return Nirvana::c_calloc <HeapBlockHdr> (num, size);
}

void* realloc (void* p, size_t size)
{
	return Nirvana::c_realloc <HeapBlockHdr> (p, size);
}

void free (void* p)
{
	Nirvana::c_free <HeapBlockHdr> (p);
}

}
