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
#include <Nirvana/POSIX.h>
#include <errno.h>
#include "impl/mbcs.h"
#include "impl/locale.h"
#include "impl/Global.h"

extern "C" size_t wcsnrtombs_l (char* dst, const wchar_t** src, size_t nwc, size_t len, __Mbstate* ps, locale_t locobj)
{
	size_t ret;
	int err = CRTL::wcsnrtombs (dst, src, nwc, len, ps, CRTL::get_cp (locobj), ret);
	if (err) {
		errno = err;
		return -1;
	}
	return ret;
}

extern "C" size_t wcsnrtombs (char* dst, const wchar_t** src, size_t nwc, size_t len, __Mbstate* ps)
{
	size_t ret;
	int err = CRTL::wcsnrtombs (dst, src, nwc, len, ps, CRTL::cur_code_page (), ret);
	if (err) {
		errno = err;
		return -1;
	}
	return ret;
}

extern "C" size_t wcsrtombs (char* dst, const wchar_t** src, size_t len, __Mbstate* ps)
{
	return wcsnrtombs (dst, src, std::numeric_limits <size_t>::max (), len, ps);
}

extern "C" size_t wcrtomb (char* s, wchar_t wc, __Mbstate* ps)
{
	if (!ps) {
		int err = CRTL::Global::get_mb_state (ps, CRTL::Global::MBS_WCRTOMB);
		if (err) {
			errno = err;
			return (size_t)-1;
		}
	}
	const wchar_t* pwc = &wc;
	return wcsnrtombs (s, &pwc, 1, MB_CUR_MAX, ps);
}

extern "C" int wctomb (char* s, wchar_t wc)
{
	__Mbstate mbs {0};
	return wcrtomb (s, wc, &mbs);
}

extern "C" size_t mbsnrtowcs_l (wchar_t* dst, const char** src, size_t nms, size_t len, __Mbstate* ps, locale_t locobj)
{
	size_t ret;
	int err = CRTL::mbsnrtowcs (dst, src, nms, len, ps, CRTL::get_cp (locobj), ret);
	if (err) {
		errno = err;
		return -1;
	}
	return ret;
}

extern "C" size_t mbsnrtowcs (wchar_t* dst, const char** src, size_t nms, size_t len, __Mbstate* ps)
{
	size_t ret;
	int err = CRTL::mbsnrtowcs (dst, src, nms, len, ps, CRTL::cur_code_page (), ret);
	if (err) {
		errno = err;
		return -1;
	}
	return ret;
}

extern "C" size_t mbsrtowcs (wchar_t* dst, const char** src, size_t len, __Mbstate* ps)
{
	return mbsnrtowcs (dst, src, std::numeric_limits <size_t>::max (), len, ps);
}

size_t mbrlen (const char* s, size_t n, __Mbstate* ps)
{
	int err = 0;
	if (!ps)
		err = CRTL::Global::get_mb_state (ps, CRTL::Global::MBS_MBRLEN);
	if (!err) {
		if (!s) {
			Nirvana::state_clear (*ps);
			return 0;
		} else {
			size_t len;
			err = CRTL::mbrtowc (nullptr, s, std::numeric_limits <size_t>::max (), ps, CRTL::cur_code_page (), len);
			if (!err)
				return len;
		}
	}
	errno = err;
	return (size_t)-1;
}

extern "C" int mblen (const char* s, size_t n)
{
	return (int)mbrlen (s, n, nullptr);
}

extern "C" int mbsinit (const __Mbstate* ps)
{
	return (ps && !Nirvana::state_empty (*ps)) ? 0 : 1;
}

extern "C" size_t mbrtowc (wchar_t* pwc, const char* s, size_t n, __Mbstate* ps)
{
	size_t len;
	int err = CRTL::mbrtowc (pwc, s, n, ps, CRTL::cur_code_page (), len);
	if (err) {
		errno = err;
		return (size_t)-1;
	}
	return len;
}

extern "C" int mbtowc (wchar_t* pwc, const char* s, size_t n)
{
	size_t len;
	int err = CRTL::mbtowc (pwc, s, n, CRTL::cur_code_page (), len);
	if (err) {
		errno = err;
		return -1;
	}
	return len;
}

extern "C" int mbtowc_l (wchar_t* pwc, const char* s, size_t n, locale_t locobj)
{
	size_t len;
	int err = CRTL::mbtowc (pwc, s, n, CRTL::get_cp (locobj), len);
	if (err) {
		errno = err;
		return -1;
	}
	return len;
}

extern "C" int wctob (wint_t wc)
{
	auto cp = CRTL::cur_code_page ();
	if (!cp) {
		if (wc <= 0x7F)
			return wc;
		else
			return EOF;
	} else {
		bool use_def;
		return cp->to_narrow (wc, EOF, use_def);
	}
}

extern "C" wint_t btowc (int c)
{
	auto cp = CRTL::cur_code_page ();
	if (!cp) {
		if (Nirvana::octet_cnt (c) == 1)
			return c;
		else
			return WEOF;
	} else {
		try {
			return cp->to_wide (c);
		} catch (...) {
			return WEOF;
		}
	}
}