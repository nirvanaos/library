/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include <string.h>

namespace CRTL {

template <typename C> inline
C* strstr (const C* haystack, const C* needle) noexcept
{
	static const size_t MAX_KMP_LEN = 2048;
	unsigned pi [MAX_KMP_LEN];
	pi [0] = 0;
	size_t kmp_l;
	for (kmp_l = 1; kmp_l < MAX_KMP_LEN; ++kmp_l) {
		C c = needle [kmp_l];
		if (!c)
			break;
		unsigned j = pi [kmp_l - 1];
		while ((j > 0) && (c != needle [j]))
			j = pi [j-1];
		if (c == needle [j])
			++j; 
		pi [kmp_l] = j;
	}

	size_t len = kmp_l;
	while (needle [len])
		++len;

	size_t j = 0; // Count of characters matched in the needle.
		// Matched the i in the haystack.
	for (size_t i = 0; haystack [i]; ++i) {
		while ((j > 0) && (haystack [i] != needle [j]))
			// Current character does not match the character in the needle.
			// Shift the needle.
      j = pi [j - 1]; // If j == 0 we reach the needle beginning and need to break.
                  
		if (haystack [i] == needle [j]) // The current match.
			++j;              // Increment matched length.
		if (j == kmp_l) {
			// Prefix found. Check tail if exist.
			size_t nk = j;
			for (size_t hk = i + 1; nk < len; ++nk, ++hk) {
				if (haystack [hk] != needle [nk])
					break;
			}
			if (nk == len)
				return const_cast <C*> (haystack + i - kmp_l + 1);
		}
	}

	return nullptr;
}

}

extern "C" {
	
char* strstr (const char* haystack, const char* needle)
{
	return CRTL::strstr (haystack, needle);
}

wchar_t* wcsstr (const wchar_t* haystack, const wchar_t* needle)
{
	return CRTL::strstr (haystack, needle);
}

}
