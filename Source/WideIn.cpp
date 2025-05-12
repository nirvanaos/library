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
#include <Nirvana/utf8.h>

namespace Nirvana {

int32_t WideInUTF8::get ()
{
	int c = bytes_.get ();
	if (EOF == c)
		return EOF;

	__Mbstate mbs { 0, 0, 0 };
	if (!push_first (mbs, c))
		conversion_error ();
	while (mbs.__octets) {
		if (!push_next (mbs, bytes_.get ()))
			conversion_error ();
	}
	return mbs.__wchar;
}

NIRVANA_NORETURN void WideInUTF8::conversion_error ()
{
	assert (false);
	throw_CODESET_INCOMPATIBLE (make_minor_errno (EILSEQ));
}

WideInCP::WideInCP (ByteIn& bytes, CodePage::_ptr_type cp) noexcept :
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
