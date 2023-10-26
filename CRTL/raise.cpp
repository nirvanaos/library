#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>
#include <signal.h>

extern "C" int raise (int sig)
{
	try {
		Nirvana::g_system->raise ((short)sig);
	} catch (const CORBA::SystemException& ex) {
		int err = Nirvana::get_minor_errno (ex.minor ());
		if (!err)
			err = EINVAL;
		return err;
	}
	return 0;
}
