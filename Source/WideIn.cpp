/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
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
#include "../../pch/pch.h"
#include <Nirvana/WideIn.h>
#include <wctype.h>

namespace Nirvana {

int32_t WideInUTF8::get ()
{
	int32_t wc;
	int c = bytes_.get ();
	if (EOF == c)
		return EOF;
	if (!(c & 0x80)) {
		// 1 octet
		wc = c;
	} else if ((c & 0xE0) == 0xC0) {
		// 2 octets
		wc = (c & 0x1F) << 6;
		wc |= get_next_octet ();
	} else if ((c & 0xF0) == 0xE0) {
		// 3 octets
		wc = (c & 0x0F) << 12;
		wc |= get_next_octet () << 6;
		wc |= get_next_octet ();
	} else if ((c & 0xF8) == 0xF0) {
		// 4 octets
		wc = (c & 0x07) << 18;
		wc |= get_next_octet () << 12;
		wc |= get_next_octet () << 6;
		wc |= get_next_octet ();
	} else {
		assert (false);
		throw_CODESET_INCOMPATIBLE (make_minor_errno (EILSEQ));
	}
	return wc;
}

unsigned WideInUTF8::get_next_octet () const
{
	int c = bytes_.get ();
	if (EOF == c) {
		assert (false);
		throw_CODESET_INCOMPATIBLE (make_minor_errno (EILSEQ));
	}
	if ((c & 0xC0) != 0x80) {
		assert (false);
		throw_CODESET_INCOMPATIBLE (make_minor_errno (EILSEQ));
	}
	return c & 0x3F;
}

WideInCP::WideInCP (ByteIn& bytes, CodePage::_ptr_type cp) :
	WideInUTF8 (bytes),
	code_page_ (cp)
{}

int32_t WideInCP::get ()
{
	if (code_page_) {
		int c = bytes_.get ();
		if (EOF == c)
			return EOF;
		return code_page_->to_wide (bytes_.get ());
	} else
		return WideInUTF8::get ();
}

}
