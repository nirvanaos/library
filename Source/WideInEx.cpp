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
#include <Nirvana/WideInEx.h>

namespace Nirvana {

WideInEx::WideInEx (WideIn& in) :
	in_ (in),
	pos_ (0)
{
	cur_ = in_.get ();
}

int32_t WideInEx::next ()
{
	if (EOF != cur_) {
		pos_ = in_.pos ();
		cur_ = in_.get ();
	}
	return cur_;
}

int32_t WideInEx::skip_space ()
{
	int32_t c = cur ();
	while (iswspace (c))
		c = next ();
	return c;
}

}
