#include <unistd.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>

using namespace Nirvana;

extern "C" ssize_t write (int fildes, const void* buf, size_t count)
{
	int err = EIO;
	try {
		g_system->write ((uint16_t)fildes, buf, count);
		return count;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)g_system->error_number () = err;
	return -1;
}
