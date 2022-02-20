#include <Nirvana/Nirvana.h>
#include <signal.h>

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Winvalid-noreturn"
#endif

extern "C"
NIRVANA_NORETURN
void abort (void)
{
	CORBA::Internal::Bridge <Nirvana::System>* br = &static_cast <CORBA::Internal::I_ptr <Nirvana::System>> (Nirvana::g_system);
	br->_epv ().epv.raise (br, SIGABRT, nullptr);
}

