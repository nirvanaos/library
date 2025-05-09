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
#include <Nirvana/mbstate_utf8.h>

namespace Nirvana {

int octet_cnt (int b) noexcept
{
	if (!(b & 0x80))
		return 1;
	else if ((b & 0xE0) == 0xC0)
		return 2;
	else if ((b & 0xF0) == 0xE0)
		return 3;
	else if ((b & 0xF8) == 0xF0)
		return 4;
	else
		return 0;
}

bool push_first (__Mbstate& mbs, int b) noexcept
{
	switch (octet_cnt (b)) {
		case 1:
			mbs.__wchar = b;
			mbs.__octets = 0;
			break;
		case 2:
			mbs.__wchar = (b & 0x1F) << 6;
			mbs.__shift = 0;
			mbs.__octets = 1;
			break;
		case 3:
			mbs.__wchar = (b & 0x0F) << 12;
			mbs.__shift = 6;
			mbs.__octets = 2;
			break;
		case 4:
			mbs.__wchar = (b & 0x07) << 18;
			mbs.__shift = 12;
			mbs.__octets = 3;
			break;
		default:
			return false;
	}

	return true;
}

bool push_next (__Mbstate& mbs, int b) noexcept
{
	assert (mbs.__octets);
	if ((b & 0xC0) != 0x80)
		return false;
	mbs.__wchar |= (b & 0x3F) << mbs.__shift;
	--mbs.__octets;
	mbs.__shift -= 6;
	return true;
}

}
