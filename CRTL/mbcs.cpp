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
#include "pch/pch.h"
#include <Nirvana/utf8.h>
#include <errno.h>
#include "impl/locale.h"

extern "C" int mblen (const char* s, size_t n)
{
	if (!s)
		return 0;
	int c = *s;
	if (!c)
		return 0;
	int ocnt = Nirvana::octet_cnt (c);
	if (ocnt <= 0)
		return -1;
	return ocnt;
}

extern "C" int mbsinit (const __Mbstate* ps)
{
	return (ps && ps->__octets) ? 0 : 1;
}

extern "C" size_t mbrtowc (wchar_t* pwc, const char* s, size_t n, __Mbstate* ps)
{
	if (!ps) {
		errno = EINVAL;
		return -1;
	}

	size_t cnt = 0;
	if (s) {
		if (n) {
			bool valid;
			if (!ps->__octets)
				valid = Nirvana::push_first (*ps, *s);
			else
				valid = Nirvana::push_next (*ps, *s);
			if (valid && ps->__wchar) {
				++cnt;
				while (--n && ps->__octets) {
					valid = Nirvana::push_next (*ps, *(++s));
					if (!valid)
						break;
					++cnt;
				}
			}
			if (!valid) {
				errno = EILSEQ;
				cnt = -1;
			} else if (ps->__octets)
				cnt = -2;
			else if (pwc)
				*pwc = ps->__wchar;
		}
	} else
		ps->__octets = 0;

	return cnt;
}

extern "C" int mbtowc (wchar_t* pwc, const char* s, size_t n)
{
	__Mbstate mbs {0};
	size_t cnt = mbrtowc (pwc, s, n, &mbs);
	int ret;
	if ((size_t)-2 == cnt) {
		errno = EILSEQ;
		ret = -1;
	} else
		ret = (int)cnt;
	return ret;
}

extern "C" int mbtowc_l (wchar_t* pwc, const char* s, size_t n, locale_t locobj)
{
	if (!s)
		return 0;

	Nirvana::CodePage::_ref_type cp = CRTL::get_cp (locobj);
	if (cp) {
		if (pwc)
			*pwc = cp->to_wide (*s);
		return 1;
	} else
		return mbtowc (pwc, s, n);
}

extern "C" int wctomb (char* s, wchar_t wc)
{
	return Nirvana::wctomb (s, wc);
}
