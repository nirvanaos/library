#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>

extern "C" int* _errno (void)
{
	return (int*)Nirvana::g_system->error_number ();
}
