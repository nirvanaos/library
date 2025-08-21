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

#include "WideIn.h"
#include "WideOut.h"
#include "basic_string.h"
#include "throw_exception.h"

namespace Nirvana {

/// Append UTF-8 to wide character container.
/// 
/// \typeparam WCont Wide character container type.
/// \param begin UTF-8 string begin.
/// \param end UTF-8 string end.
/// \param append_to Wide character container to append converted data.
template <class WCont>
void append_wide (const char* begin, const char* end, WCont& append_to)
{
	append_to.reserve (append_to.size () + (end - begin));
	ByteInBuf bytes (begin, end);
	WideInUTF8 in (bytes);
	WideOutContainer <WCont> out (append_to);
	copy (in, out);
}

/// Append wide string to an UTF-8 container.
///
/// \typeparam WC Wide string character type.
/// \typeparam U8Cont UTF-8 container type.
/// \param begin Wide string begin.
/// \param end Wide string end.
/// \param append_to UTF-8 container to append converted data.
template <typename WC, class U8Cont>
void append_utf8 (const WC* begin, const WC* end, U8Cont& append_to)
{
	append_to.reserve (append_to.size () + (end - begin));
	ByteOutContainer <U8Cont> bytes (append_to);
	WideInBuf <WC> in (begin, end);
	WideOutUTF8 out (bytes);
	copy (in, out);
}

/// Append UTF-8 to wide container.
/// 
/// \typeparam U8Cont UTF-8 container type.
/// \typeparam WCont Wide character container type.
/// \param s UTF-8 string.
/// \param append_to Wide string to append converted data.
template <class U8Cont, class WCont>
void append_wide (const U8Cont& s, WCont& append_to)
{
	const char* p = s.data ();
	append_wide (p, p + s.size (), append_to);
}

/// Append wide string to UTF-8.
/// 
/// \typeparam WCont Wide character container type.
/// \typeparam U8Cont UTF-8 container type.
/// \param ws Wide string.
/// \param append_to UTF-8 container to append converted data.
template <class WCont, class U8Cont>
void append_utf8 (const WCont& ws, U8Cont& append_to)
{
	const auto* p = ws.data ();
	append_utf8 (p, p + ws.size (), append_to);
}

/// Append UTF-8 to wide string.
/// 
/// \typeparam WCont Wide character container type.
/// \param p UTF-8 string.
/// \param append_to Container to append converted data.
template <typename WCont>
void append_wide (const char* p, WCont& append_to)
{
	append_wide (p, p + strlen (p), append_to);
}

/// Append wide string to UTF-8 string.
/// 
/// \typeparam WC Wide string character type.
/// \typeparam U8Cont UTF-8 container type.
/// \param p Wide string.
/// \param append_to UTF-8 container to append converted data.
template <typename WC, class U8Cont>
void append_utf8 (const WC* p, U8Cont& append_to)
{
	append_utf8 (p, p + std::char_traits <WC>::length (p), append_to);
}

}

#endif
