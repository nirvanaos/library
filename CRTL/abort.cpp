#include <Nirvana/Nirvana.h>
#include <signal.h>

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Winvalid-noreturn"
#endif

extern "C"
NIRVANA_NORETURN
void abort (void)
{
	Nirvana::g_system->raise (SIGABRT);
}

