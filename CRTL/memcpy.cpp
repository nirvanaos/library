#include <Nirvana/Nirvana.h>

using namespace Nirvana;

extern "C"
void* memcpy (void* dst, const void* src, size_t count)
{
	g_memory->copy (dst, const_cast <void*> (src), count, Memory::SIMPLE_COPY);
	return dst;
}
