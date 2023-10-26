#include <string.h>

#if defined(_MSC_BUILD) && !(defined (__GNUG__) || defined (__clang__))
#pragma function(strcpy)
#endif

extern "C" char* strcpy (char* dst, const char* src)
{
	return (char*)memcpy (dst, src, strlen (src) + 1);
}
