/// \file
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
#ifndef NIRVANA_STRING_CONV_H_
#define NIRVANA_STRING_CONV_H_
#pragma once

#include "UTF8.h"
#include "basic_string.h"
#include "throw_exception.h"
#include <limits>

namespace Nirvana {

template <typename WC, class A>
void utf8_to_wide (const char* begin, const char* end, std::basic_string <WC, std::char_traits <WC>, A>& append_to)
{
	append_to.reserve (append_to.size () + (end - begin));
	while (begin < end) {
		uint32_t c = utf8_to_utf32 (begin, end);
		if (sizeof (WC) < sizeof (uint32_t) && ((~(uint32_t)0 << (sizeof (WC) * 8)) & c))
			throw_DATA_CONVERSION ();
		append_to += (WC)c;
	}
}

template <typename WC, class A>
void wide_to_utf8 (const WC* begin, const WC* end, std::basic_string <char, std::char_traits <char>, A>& append_to)
{
	append_to.reserve (append_to.size () + (end - begin));
	const char* cend = append_to.data () + append_to.size ();
	while (begin < end) {
		size_t cc = cend - append_to.data ();
		append_to.resize (cc + 4);
		char* p = const_cast <char*> (append_to.data () + cc);
		cend = utf32_to_utf8 (*(begin++), p, append_to.data () + append_to.size ());
	}
	append_to.resize (cend - append_to.data ());
}

template <typename WC, class A1, class A2>
void utf8_to_wide (const std::basic_string <char, std::char_traits <char>, A1>& s, std::basic_string <WC, std::char_traits <WC>, A2>& append_to)
{
	const char* p = s.data ();
	utf8_to_wide (p, p + s.size (), append_to);
}

template <typename WC, class A1, class A2>
void wide_to_utf8 (const std::basic_string <WC, std::char_traits <WC>, A1>& ws, std::basic_string <char, std::char_traits <char>, A2>& append_to)
{
	const WC* p = ws.data ();
	wide_to_utf8 (p, p + ws.size (), append_to);
}

}

#endif
