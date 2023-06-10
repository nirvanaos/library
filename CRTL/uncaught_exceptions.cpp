#include <exception>

extern "C" int __cdecl __uncaught_exceptions ();

namespace std {

#ifdef NIRVANA_C17

int uncaught_exceptions () noexcept
{
	return __uncaught_exceptions ();
}

#else

bool uncaught_exception () noexcept
{
	return __uncaught_exceptions () > 0;
}

#endif

}
