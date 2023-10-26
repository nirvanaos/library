#include <string.h>
#include <Nirvana/strcpy_s.h>

extern "C"
errno_t strcpy_s (char* dst, rsize_t dst_size, const char* src)
{
	return Nirvana::strcpy_s (dst, dst_size, src);
}

extern "C"
errno_t wcscpy_s (wchar_t* dst, rsize_t dst_size, const wchar_t* src)
{
	return Nirvana::strcpy_s (dst, dst_size, src);
}
