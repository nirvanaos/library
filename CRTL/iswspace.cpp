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
#include <stdint.h>
#include <algorithm>

extern "C"
int iswspace (unsigned short ch)
{
	static const unsigned short spaces [] = {
		0x0009, // character tabulation
		0x000A, // line feed
		0x000B, // line tabulation
		0x000C, // form feed
		0x000D, // carriage return
		0x0020, // space
		0x0085, // next line
		0x00A0, // no - break space
		0x1680, // ogham space mark
		0x180E, // mongolian vowel separator
		0x2000, // en quad
		0x2001, // em quad
		0x2002, // en space
		0x2003, // em space
		0x2004, // three - per - em space
		0x2005, // four - per - em space
		0x2006, // six - per - em space
		0x2007, // figure space
		0x2008, // punctuation space
		0x2009, // thin space
		0x200A, // hair space
		0x200B, // zero width space
		0x200C, // zero width non - joiner
		0x200D, // zero width joiner
		0x2028, // line separator
		0x2029, // paragraph separator
		0x202F, // narrow no - break space
		0x205F, // medium mathematical space
		0x2060, // word joiner
		0x3000, // ideographic space
		0xFEFF  // zero width non - breaking space
	};

	const unsigned short* f = std::lower_bound (spaces, std::end (spaces), ch);
	return f != std::end (spaces) && ch == *f;
}
