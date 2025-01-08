/*
* Nirvana C runtime library.
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
#include "pch/pch.h"
#include <Nirvana/strtoi.h>
#include <errno.h>

namespace CRTL {

using namespace Nirvana;

template <typename I>
I strtoi (const char* __restrict s, char** __restrict ptr, int base) noexcept
{
	I ret;
	try {
		ByteInStr bytes (s);
		WideInUTF8 wide (bytes);
		WideInEx in (wide);
		Nirvana::strtoi (in, ret, base);
		if (ptr)
			*ptr = const_cast <char*> (bytes.cur_ptr ());
	} catch (const CORBA::SystemException& ex) {
		int err = get_minor_errno (ex.minor ());
		if (!err)
			err = EINVAL;
		errno = err;
	} catch (...) {
		errno = EINVAL;
	}
	return ret;
}

}

extern "C"
long strtol (const char* __restrict s, char** __restrict ptr, int base)
{
	return CRTL::strtoi <long> (s, ptr, base);
}

extern "C"
long long strtoll (const char* __restrict s, char** __restrict ptr, int base)
{
	return CRTL::strtoi <long long> (s, ptr, base);
}

extern "C"
unsigned long strtoul (const char* __restrict s, char** __restrict ptr, int base)
{
	return CRTL::strtoi <unsigned long> (s, ptr, base);
}

extern "C"
unsigned long long strtoull (const char* __restrict s, char** __restrict ptr, int base)
{
	return CRTL::strtoi <long long> (s, ptr, base);
}

extern "C"
int atoi (const char* s)
{
	return CRTL::strtoi <int> (s, nullptr, 10);
}

extern "C"
long atol (const char* s)
{
	return CRTL::strtoi <long> (s, nullptr, 10);
}
