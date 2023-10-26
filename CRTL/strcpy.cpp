#include <string.h>

extern "C" char* strcpy (char* dst, const char* src)
{
	return (char*)memcpy (dst, src, strlen (src) + 1);
}
