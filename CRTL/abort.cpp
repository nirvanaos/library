#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>
#include <signal.h>

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Winvalid-noreturn"
#endif

using namespace CORBA::Internal;
using namespace Nirvana;

extern "C"
NIRVANA_NORETURN
void abort (void)
{
	Bridge <System>* br = 
		static_cast <Bridge <System>*> (&static_cast <CORBA::Internal::I_ptr <Nirvana::System> > (Nirvana::g_system));
	br->_epv ().epv.raise (br, SIGABRT, nullptr);
}

