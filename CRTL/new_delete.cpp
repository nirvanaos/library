#include <Nirvana/Nirvana.h>

using namespace Nirvana;

void* operator new (size_t cb)
{
	return g_memory->allocate (nullptr, cb, 0);
}

void operator delete (void* p, size_t cb) NIRVANA_NOEXCEPT
{
	g_memory->release (p, cb);
}
