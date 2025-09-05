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
#include "UTF8In.h"
#include <string.h>
#include <wchar.h>

namespace CRTL {

UTF8In::UTF8In (const char* s, size_t n) noexcept :
	bytes_ (s),
	len_ (strnlen (s, n))
{}

UTF8In::UTF8In (const char* s) noexcept :
	bytes_ (s),
	len_ (strlen (s))
{}

wint_t UTF8In::get () noexcept
{
	if (len_) {
		mbstate_t mbs {0};
		wchar_t wc;
		size_t cnt = mbrtowc (&wc, bytes_, len_, &mbs);
		if (cnt != (size_t)-1 && cnt != (size_t)-2) {
			bytes_ += cnt;
			len_ -= cnt;
			return wc;
		}
	}
	return 0;
}

}