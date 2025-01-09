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
#include <Nirvana/ByteIn.h>

namespace Nirvana {

int ByteInStr::get ()
{
	int c = (unsigned char)*p_;
	if (c)
		++p_;
	else
		c = EOF;
	return c;
}

int ByteInBuf::get ()
{
	if (p_ >= end_)
		return EOF;
	else
		return (unsigned char)*(p_++);
}

int ByteInFile::get ()
{
	int c = fgetc (f_);
	if (EOF == c) {
		if (!feof (f_))
			throw CORBA::UNKNOWN (make_minor_errno (ferror (f_)));
	} else
		++pos_;
	return c;
}

}
