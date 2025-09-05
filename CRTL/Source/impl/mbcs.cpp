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
#include "mbcs.h"
#include <errno.h>
#include <Nirvana/utf8.h>
#include "Global.h"

namespace CRTL {

int wcsnrtombs (char* dst, const wchar_t** src, size_t nwc, size_t len, __Mbstate* ps,
	Nirvana::CodePage::_ptr_type cp, size_t& count) noexcept
{
	if (!src)
		return EINVAL;

	if (!ps) {
		int err = Global::get_mb_state (ps, CRTL::Global::MBS_WCSNRTOMBS);
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
		int err = Global::get_mb_state (ps, CRTL::Global::MBS_MBSRTOWCS);
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

int mbrtowc (wchar_t* pwc, const char* s, size_t n, __Mbstate* ps,
	Nirvana::CodePage::_ptr_type cp, size_t& count) noexcept
{
	int err = 0;
	if (!ps)
		err = Global::get_mb_state (ps, Global::MBS_MBRTOWC);
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
	Nirvana::CodePage::_ptr_type cp, size_t& count) noexcept
{
	__Mbstate mbs {0};
	size_t cnt;
	int err = mbrtowc (pwc, s, n, &mbs, cp, cnt);
	if (!err && (size_t)-2 == cnt)
		err = EILSEQ; // Incomplete sequence is an error
	return err;
}

} // namespace CRTL

