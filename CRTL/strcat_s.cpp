#include <Nirvana/Nirvana.h>
#include <string.h>
#include <CRTL/impl/strlen.h>

namespace CRTL {

template <typename C> inline
errno_t strcat_s (C* dst, rsize_t dst_size, const C* src)
{
	if (!dst || !src)
		return EINVAL;
	if (dst <= src && src < dst + dst_size)
		return EINVAL;
	size_t dst_len = strnlen (dst, dst_size);
	if (dst_len >= dst_size)
		return ERANGE;
	size_t src_max = dst_size - dst_len;
	size_t src_len = strnlen (src, src_max);
	if (src_len >= src_max)
		return ERANGE;

	++src_len;
	size_t cb = src_len * sizeof (C);
	Nirvana::g_memory->copy (dst + dst_len, const_cast <C*> (src), cb, 0);
	return 0;
}

}

extern "C"
errno_t strcat_s (char* dst, rsize_t dst_size, const char* src)
{
	return CRTL::strcat_s (dst, dst_size, src);
}

extern "C"
errno_t wcscat_s (wchar_t* dst, rsize_t dst_size, const wchar_t* src)
{
	return CRTL::strcat_s (dst, dst_size, src);
}
