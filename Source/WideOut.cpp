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
#include <Nirvana/WideOut.h>
#include <Nirvana/WideIn.h>

namespace Nirvana {

void WideOutUTF8::put (uint32_t wc)
{
	if (wc <= 0x7F) {
		// 1 octet
		bytes_.put ((unsigned)wc);
	} else if (wc <= 0x7FF) {
		// 2 octets
		bytes_.put ((((unsigned)wc >> 6) & 0x1F) | 0xC0);
		bytes_.put (((unsigned)wc & 0x3F) | 0x80);
	} else if (wc <= 0xFFFF) {
		// 3 octets
		bytes_.put ((((unsigned)wc >> 12) & 0x0F) | 0xE0);
		bytes_.put ((((unsigned)wc >> 6) & 0x3F) | 0x80);
		bytes_.put (((unsigned)wc & 0x3F) | 0x80);
	} else if (wc <= 0x0010FFFF) {
		// 4 octets
		bytes_.put ((unsigned)((wc >> 18) & 0x07) | 0xF0);
		bytes_.put ((((unsigned)wc >> 12) & 0x3F) | 0x80);
		bytes_.put ((((unsigned)wc >> 6) & 0x3F) | 0x80);
		bytes_.put (((unsigned)wc & 0x3F) | 0x80);
	} else {
		assert (false);
		throw_CODESET_INCOMPATIBLE (make_minor_errno (EILSEQ));
	}
}

WideOutCP::WideOutCP (ByteOut& bytes, CodePage::_ptr_type cp) noexcept :
	WideOutUTF8 (bytes),
	code_page_ (cp)
{}

void WideOutCP::put (uint32_t wc)
{
	if (code_page_) {
		bool used_def;
		bytes_.put (code_page_->to_narrow (wc, CodePage::NO_DEFAULT, used_def));
	} else
		WideOutUTF8::put (wc);
}

WideOutEx::WideOutEx (WideOut& out) :
	out_ (out),
	pos_ (0)
{}

void WideOutEx::put (uint32_t wc)
{
	out_.put (wc);
	++pos_;
}

void copy (WideIn& in, WideOut& out)
{
	for (;;) {
		auto c = in.get ();
		if (EOF == c)
			break;
		out.put (c);
	}
}

}
