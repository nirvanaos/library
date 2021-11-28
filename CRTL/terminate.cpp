#include <Nirvana/Nirvana.h>

extern "C"
#ifdef _MSC_BUILD
__declspec(noreturn)
#else
NIRVANA_NORETURN
#endif
void terminate (void)
{
	abort ();
}
