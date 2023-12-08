#include <string.h>
#include <wchar.h>
#include "strlen.h"

#if defined(_MSC_BUILD) && !(defined (__GNUG__) || defined (__clang__))
#pragma function(strcmp)
#pragma function(wcscmp)
#endif

namespace CRTL {

template <typename C> inline
int strcmp (const C* ls, const C* rs)
{
	/* If s1 or s2 are unaligned, then compare bytes. */
	if (sizeof (Word) > sizeof (C) && !unaligned (ls) && !unaligned (rs)) {
		/* If s1 and s2 are word-aligned, compare them a word at a time. */
		const Word* lw = (const Word*)ls;
		const Word* rw = (const Word*)rs;
		while (*lw == *rw) {
			/* To get here, *lw == *rw, thus if we find a null in *lw,
			then the strings must be equal, so return zero.  */
			if (detect_null <sizeof (C)> (*lw))
				return 0;

			++lw;
			++rw;
		}

		/* A difference was detected in last few bytes of ls, so search bytewise */
		ls = (const C*)lw;
		rs = (const C*)rw;
	}

	while (*ls != '\0' && *ls == *rs) {
		++ls;
		++rs;
	}
	return ((unsigned)*ls - (unsigned)*rs);
}

}

extern "C"
int strcmp (const char* ls, const char* rs)
{
	return CRTL::strcmp (ls, rs);
}

extern "C"
int wcscmp (const wchar_t* ls, const wchar_t* rs)
{
	return CRTL::strcmp (ls, rs);
}
