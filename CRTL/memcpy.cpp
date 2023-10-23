#include <Nirvana/Nirvana.h>

using namespace Nirvana;

static const size_t MAX_REAL_COPY = 1024;

extern "C"
void* memcpy (void* dst, const void* src, size_t count)
{
	if (count <= MAX_REAL_COPY)
		real_copy ((const void*)src, (const void*)((const uint8_t*)src + count), dst);
	else
		g_memory->copy (dst, const_cast <void*> (src), count, Memory::SIMPLE_COPY);
	return dst;
}

extern "C"
void* memmove (void* dst, const void* src, size_t count)
{
	if (count <= MAX_REAL_COPY)
		real_move ((const void*)src, (const void*)((const uint8_t*)src + count), dst);
	else
		g_memory->copy (dst, const_cast <void*> (src), count, Memory::SIMPLE_COPY);
	return dst;
}
