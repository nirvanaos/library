#include <stdlib.h>

#if defined(_MSC_BUILD) && !(defined (__GNUG__) || defined (__clang__))
#pragma function(llabs)
#endif

extern "C" int abs (int const number)
{
	return number >= 0 ? number : -number;
}

extern "C" long long __cdecl llabs (long long const number)
{
	return number >= 0 ? number : -number;
}
