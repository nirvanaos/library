#include <stdlib.h>

extern "C" NIRVANA_NORETURN void exit (int)
{
	abort ();
}