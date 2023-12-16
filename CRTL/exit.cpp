#include <Nirvana/NirvanaBase.h>

extern "C" NIRVANA_NORETURN void abort ();

extern "C" NIRVANA_NORETURN void exit (int)
{
	abort ();
}
