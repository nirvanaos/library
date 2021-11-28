#include <Nirvana/Nirvana.h>

using namespace Nirvana;

extern "C" int* __errno (void)
{
	return (int*)g_system->error_number ();
}
