#ifndef NIRVANA_CRTL_UTILS_H_
#define NIRVANA_CRTL_UTILS_H_

#include <stdint.h>

namespace Nirvana {

/// The machine word
typedef size_t Word;

/// \returns Nonzero if w contains a null character
template <size_t char_size>
Word detect_null (Word w);

template <>
inline Word detect_null <1> (Word w)
{
	return ((w - (Word)0x0101010101010101ull) & ~w & (Word)0x8080808080808080ull);
}

template <>
inline Word detect_null <2> (Word w)
{
	return ((w - (Word)0x0001000100010001ull) & ~w & (Word)0x8000800080008000ull);
}

template <>
inline Word detect_null <4> (Word w)
{
	return ((w - 0x0001000100010001ull) & ~w & 0x8000800080008000ull);
}

inline uintptr_t unaligned (const void* p)
{
	return (uintptr_t)p & (sizeof (Word) - 1);
}

template <typename C> inline
size_t strlen (const C* s)
{
	const C* p = s;
	if (sizeof (Word) > sizeof (C) && !unaligned (p)) {
		/* If the string is word-aligned, we can check for the presence of
		 a null in each word-sized block.  */
		const Word* wp = (const Word*)p;
		while (!detect_null <sizeof (C)> (*wp))
			++wp;
		/* Once a null is detected, we check each byte in that block for a
		 precise position of the null.  */
		p = (const C*)wp;
	}
	while (*p)
		++p;
	return p - s;
}

}

#endif
