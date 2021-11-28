#include <exception>

extern "C" int __cdecl __uncaught_exceptions ();

namespace std {

#ifdef NIRVANA_C17

int uncaught_exceptions () NIRVANA_NOEXCEPT
{
	return __uncaught_exceptions ();
}

#else

bool uncaught_exception () NIRVANA_NOEXCEPT
{
	return __uncaught_exceptions () > 0;
}

#endif

}
