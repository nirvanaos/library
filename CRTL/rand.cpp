#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>

extern "C" void srand (unsigned int seed)
{
	Nirvana::g_system->srand ((uint32_t)seed);
}

extern "C" int rand (void)
{
	return Nirvana::g_system->rand ();
}
