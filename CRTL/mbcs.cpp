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
#include <Nirvana/POSIX.h>
#include <errno.h>
#include "impl/locale.h"
#include "impl/Global.h"

namespace CRTL {

int wcsnrtombs (char* dst, const wchar_t** src, size_t nwc, size_t len, __Mbstate* ps,
	Nirvana::CodePage::_ptr_type cp, size_t& count) noexcept
{
	if (!src)
		return EINVAL;

	if (!ps) {
		int err = global.get_mb_state (ps, CRTL::Global::MBS_WCSNRTOMBS);
		if (err)
			return err;
	}
		
  // If no dst pointer, treat len as maximum possible value.
	if (!dst)
		len = std::numeric_limits <size_t>::max ();

	const wchar_t* psrc = *src;
	const wchar_t* end = psrc + nwc;
	size_t cnt = 0;
	int err = 0;

	try {
		for (;;) {

			while (ps->__octets && cnt < len) {
				char c = Nirvana::pop_octet (*ps);
				if (dst)
					*(dst++) = c;
				++cnt;
			}

			if (psrc >= end)
				break;

			wchar_t wc = *psrc;
			if (!wc) {
				if (dst && cnt < len)
					*dst = 0;
				Nirvana::state_clear (*ps);
				break;
			}

			if (cp) {
				bool dfl;
				ps->__wchar = cp->to_narrow (wc, Nirvana::CodePage::NO_DEFAULT, dfl);
				ps->__octets = 1;
			} else if (!Nirvana::push_wide (*ps, wc)) {
				err = EILSEQ;
				break;
			}
			++psrc;
		}
	} catch (...) {
		err = EILSEQ;
	}
	count = cnt;
	*src = psrc;
	return err;
}

int mbsnrtowcs (wchar_t* dst, const char** src, size_t nms, size_t len, __Mbstate* ps,
	Nirvana::CodePage::_ptr_type cp, size_t& count) noexcept
{
	if (!src)
		return EINVAL;

	if (!ps) {
		int err = global.get_mb_state (ps, CRTL::Global::MBS_MBSRTOWCS);
		if (err)
			return err;
	}
		
  // If no dst pointer, treat len as maximum possible value.
	if (!dst)
		len = std::numeric_limits <size_t>::max ();

	const char* psrc = *src;
	size_t cnt = 0;
	int err = 0;

	try {
		for (;;) {
			if (!ps->__octets && ps->__wchar && len) {
				if (dst)
					*(dst++) = ps->__wchar;
				--len;
				ps->__wchar = 0;
			}

			if (cnt >= nms)
				break;

			char c = *psrc;

			if (!c) {
				if (ps->__octets)
					err = EILSEQ; // Missing octets before the terminator
				else {
					if (dst)
						*dst = 0;
					Nirvana::state_clear (*ps);
				}
				break;
			}

			if (cp) {
				ps->__wchar = cp->to_wide (c);
				ps->__octets = 0;
			} else if (!(ps->__octets ? Nirvana::push_next (*ps, c) : Nirvana::push_first (*ps, c))) {
				err = EILSEQ;
				break;
			}
			++psrc;
			++cnt;
		}
	} catch (...) {
		err = EILSEQ;
	}

	count = cnt;
	*src = psrc;
	return err;
}

Nirvana::CodePage::_ptr_type cur_code_page () noexcept
{
	try {
		return Nirvana::CodePage::_downcast (Nirvana::the_posix->cur_locale ()->get_facet (LC_CTYPE));
  } catch (...) {
    return nullptr;
  }
}

int mbrtowc (wchar_t* pwc, const char* s, size_t n, __Mbstate* ps,
	Nirvana::CodePage::_ptr_type cp, size_t& count)
{
	int err = 0;
	if (!ps)
		err = global.get_mb_state (ps, Global::MBS_MBRTOWC);
	if (!err) {
		if (!s) {
			s = "";
			n = 1;
		}
		size_t len;
		err = mbsnrtowcs (nullptr, &s, n, 0, ps, cp, len);
		if (!err) {
			if (ps->__octets)
				count = (size_t)-2; // Incomplete character
			else
				count = len;
		}
	}
	return err;
}

int mbtowc (wchar_t* pwc, const char* s, size_t n,
	Nirvana::CodePage::_ptr_type cp, size_t& count)
{
	__Mbstate mbs {0};
	size_t cnt;
	int err = mbrtowc (pwc, s, n, &mbs, cp, cnt);
	if (!err && (size_t)-2 == cnt)
		err = EILSEQ; // Incomplete sequence is an error
	return err;
}

} // namespace CRTL

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
		int err = CRTL::global.get_mb_state (ps, CRTL::Global::MBS_WCRTOMB);
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
		err = CRTL::global.get_mb_state (ps, CRTL::Global::MBS_MBRLEN);
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