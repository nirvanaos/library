#include <string.h>
#include <wchar.h>

#if defined(_MSC_BUILD) && !(defined (__GNUG__) || defined (__clang__))
#pragma function(strcpy)
#endif

extern "C" char* strcpy (char* dst, const char* src)
{
	return (char*)memcpy (dst, src, strlen (src) + 1);
}

extern "C" wchar_t* wcscpy (wchar_t* dst, const wchar_t* src)
{
	return (wchar_t*)memcpy (dst, src, (wcslen (src) + 1) * sizeof (wchar_t));
}
