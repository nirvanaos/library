#include <string.h>
#include "utils.h"

#if defined(_MSC_BUILD) && !(defined (__GNUG__) || defined (__clang__))
#pragma function (strlen)
//#pragma function (wcslen)
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
