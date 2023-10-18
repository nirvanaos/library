
#include <string.h>
#include <wchar.h>
#include <Nirvana/strlen.h>

#if defined(_MSC_BUILD) && !(defined (__GNUG__) || defined (__clang__))
#pragma function (strlen)
#pragma function (wcslen)
#endif

extern "C"
size_t strlen (const char* s)
{
	return Nirvana::strlen (s);
}

extern "C"
size_t wcslen (const wchar_t* s)
{
	return Nirvana::strlen (s);
}

extern "C"
size_t strnlen (const char* s, size_t maxlen)
{
	return Nirvana::strnlen (s, maxlen);
}

extern "C"
size_t wcsnlen (const wchar_t* s, size_t maxlen)
{
	return Nirvana::strnlen (s, maxlen);
}
