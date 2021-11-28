#include <Nirvana/Nirvana.h>
#include <string.h>
#include "utils.h"

namespace Nirvana {

template <typename C> inline
errno_t strcpy_s (C* dst, rsize_t dst_size, const C* src)
{
	if (!dst)
		return EINVAL;
	if (!src) {
		dst [0] = 0;
		return EINVAL;
	}
	size_t src_size = strlen (src) + 1;
	if (dst_size < src_size) {
		dst [0] = 0;
		return ERANGE;
	}
	g_memory->copy (dst, const_cast <C*> (src), src_size * sizeof (C), 0);
	return 0;
}

}

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
