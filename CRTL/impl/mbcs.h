/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#ifndef CRTL_IMPL_MBCS_H_
#define CRTL_IMPL_MBCS_H_
#pragma once

#include <CORBA/CORBA.h>
#include <Nirvana/nls.h>
#include <Nirvana/mbstate.h>

namespace CRTL {

int wcsnrtombs (char* dst, const wchar_t** src, size_t nwc, size_t len, __Mbstate* ps,
	Nirvana::CodePage::_ptr_type cp, size_t& count) noexcept;

int mbsnrtowcs (wchar_t* dst, const char** src, size_t nms, size_t len, __Mbstate* ps,
	Nirvana::CodePage::_ptr_type cp, size_t& count) noexcept;

int mbrtowc (wchar_t* pwc, const char* s, size_t n, __Mbstate* ps,
	Nirvana::CodePage::_ptr_type cp, size_t& count) noexcept;

int mbtowc (wchar_t* pwc, const char* s, size_t n,
	Nirvana::CodePage::_ptr_type cp, size_t& count) noexcept;

}

#endif
