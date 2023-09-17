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

/// Convert UTF-8 to wide string.
/// 
/// \typeparam WC Wide string character type.
/// \typeparam A Wide string allocator type.
/// \param begin UTF-8 string begin.
/// \param end UTF-8 string end.
/// \param append_to Wide string to append converted data.
/// \throws CORBA::DATA_CONVERSION if input UTF-8 character can not be represented as WC type.
template <typename WC, class A>
void append_wide (const char* begin, const char* end, std::basic_string <WC, std::char_traits <WC>, A>& append_to)
{
	append_to.reserve (append_to.size () + (end - begin));
	while (begin < end) {
		uint32_t c = utf8_to_utf32 (begin, end);
		if (sizeof (WC) < sizeof (uint32_t) && ((~(uint32_t)0 << (sizeof (WC) * 8)) & c))
			throw_CODESET_INCOMPATIBLE (make_minor_errno (EILSEQ));
		append_to += (WC)c;
	}
}

/// Convert wide string to UTF-8 string.
/// 
/// \typeparam WC Wide string character type.
/// \typeparam A UTF-8 string allocator type.
/// \param begin Wide string begin.
/// \param end Wide string end.
/// \param append_to UTF-8 string to append converted data.
template <typename WC, class A>
void append_utf8 (const WC* begin, const WC* end, std::basic_string <char, std::char_traits <char>, A>& append_to)
{
	if (begin < end) {
		size_t converted_size = append_to.size ();
		append_to.reserve (converted_size + (end - begin) + 3);
		try {
			do {
				append_to.resize (converted_size + 4);
				char* dst = const_cast <char*> (append_to.data () + converted_size);
				converted_size += utf32_to_utf8 (*begin, dst, dst + 4) - dst;
			} while (end > ++begin);
		} catch (...) {
			append_to.resize (converted_size);
			throw;
		}
		append_to.resize (converted_size);
	}
}

/// Convert UTF-8 to wide string.
/// 
/// \typeparam S UTF-8 string type.
/// \typeparam WC Wide string character type.
/// \typeparam A Wide string allocator type.
/// \param s UTF-8 string.
/// \param end UTF-8 string end.
/// \param append_to Wide string to append converted data.
/// \throws CORBA::DATA_CONVERSION if input UTF-8 character can not be represented as WC type.
template <class S, typename WC, class A>
void append_wide (const S& s, std::basic_string <WC, std::char_traits <WC>, A>& append_to)
{
	const char* p = s.data ();
	append_wide (p, p + s.size (), append_to);
}

/// Convert wide string to UTF-8 string.
/// 
/// \typeparam WS Wide string type.
/// \typeparam A UTF-8 string allocator type.
/// \param ws Wide string.
/// \param append_to UTF-8 string to append converted data.
template <class WS, class A>
void append_utf8 (const WS& ws, std::basic_string <char, std::char_traits <char>, A>& append_to)
{
	const auto* p = ws.data ();
	append_utf8 (p, p + ws.size (), append_to);
}

/// Convert UTF-8 to wide string.
/// 
/// \typeparam WC Wide string character type.
/// \typeparam A Wide string allocator type.
/// \param p UTF-8 string.
/// \param append_to Wide string to append converted data.
/// \throws CORBA::DATA_CONVERSION if input UTF-8 character can not be represented as WC type.
template <typename WC, class A>
void append_wide (const char* p, std::basic_string <WC, std::char_traits <WC>, A>& append_to)
{
	append_wide (p, p + strlen (p), append_to);
}

/// Convert wide string to UTF-8 string.
/// 
/// \typeparam WS Wide string type.
/// \typeparam A UTF-8 string allocator type.
/// \param p Wide string.
/// \param append_to UTF-8 string to append converted data.
template <typename WC, class A>
void append_utf8 (const WC* p, std::basic_string <char, std::char_traits <char>, A>& append_to)
{
	append_utf8 (p, p + std::char_traits <WC>::length (p), append_to);
}

}

#endif
