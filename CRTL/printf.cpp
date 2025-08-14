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
#include "pch/pch.h"
#include <stdio.h>
#include <wchar.h>
#include <limits>
#include <Nirvana/printf.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/locale_defs.h>
#include "impl/ByteOutFile.h"

using namespace Nirvana;

namespace CRTL {

/// \brief Generalized C-style formatting function.
/// As it intended to C formatting, it does not throw exceptions
/// but sets `errno` codes on error instead.
/// 
/// \typeparam C Character type.
/// \param fmt Format string.
/// \param args Arguments.
/// \param out Output stream.
/// \param loc Locale conversion settings.
/// \returns Number of characters transmitted to the output stream or negative value if an output
///          error or an encoding error (for string and character conversion specifiers) occurred.
template <class C>
int vprintf (const C* fmt, va_list args, WideOut& out, const struct lconv* loc) noexcept
{
	size_t ret;
	int err = Nirvana::vprintf (fmt, args, out, ret, loc);
	if (err) {
		errno = err;
		return -1;
	} else
		return (int)ret;
}

/// \brief Generalized vsnprintf implementation
/// As it intended to C formatting, it does not throw exceptions
/// but sets `errno` codes on error instead.
/// 
/// \typeparam C Character type.
/// \param buffer Output buffer pointer.
/// \param bufsz Output buffer size.
/// \param fmt Format string.
/// \param args Arguments.
/// \param loc Locale conversion settings.
/// \returns Number of characters (not including the terminating null character) which would have
///   been written to buffer if bufsz was ignored or -1 on error.
template <class C>
int vsnprintf (C* buffer, size_t bufsz, const C* fmt, va_list args) noexcept
{
	size_t ret;
	int err = Nirvana::vsnprintf (buffer, bufsz, fmt, args, ret,
		Nirvana::the_posix->cur_locale ()->localeconv ());
	if (err) {
		errno = err;
		return -1;
	} else
		return (int)ret;
}

template <class C>
int vasprintf (C** strp, const C* fmt, va_list args)
{
	const struct lconv* lc = Nirvana::the_posix->cur_locale ()->localeconv ();
	size_t cnt;
	int err = Nirvana::vsnprintf ((C*)nullptr, 0, fmt, args, cnt, lc);
	C* msg = nullptr;
	if (!err) {
		msg = (C*)malloc ((cnt + 1) * sizeof (C));
		if (!msg)
			err = ENOMEM;
		else
			err = Nirvana::vsnprintf (msg, cnt + 1, fmt, args, cnt, lc);
	}
	*strp = msg;
	if (err) {
		errno = err;
		return -1;
	} else
		return (int)cnt;
}

}

using namespace CRTL;

extern "C" int vfprintf (FILE* stream, const char* fmt, va_list args)
{
	File* f = File::cast (stream);
	if (!f)
		return -1;
	
	auto loc = Nirvana::the_posix->cur_locale ();
	CodePage::_ref_type code_page = CodePage::_downcast (loc->get_facet (LC_CTYPE));
	ByteOutFile file_bytes (f);
	WideOutCP out (file_bytes, code_page);

	return CRTL::vprintf (fmt, args, out, loc->localeconv ());
}

extern "C" int vprintf (const char* fmt, va_list args)
{
	return vfprintf (stdout, fmt, args);
}

extern "C" int fprintf (FILE* stream, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vfprintf (stream, fmt, args);
	va_end (args);
	return ret;
}

extern "C" int printf (const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vprintf (fmt, args);
	va_end (args);
	return ret;
}

extern "C" int vsnprintf (char* buffer, size_t bufsiz, const char* fmt, va_list args)
{
	return CRTL::vsnprintf (buffer, bufsiz, fmt, args);
}

extern "C" int vsprintf (char* buffer, const char* fmt, va_list args)
{
	return vsnprintf (buffer, std::numeric_limits <size_t>::max (), fmt, args);
}

extern "C" int snprintf (char* buffer, size_t bufsiz, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vsnprintf (buffer, bufsiz, fmt, args);
	va_end (args);
	return ret;
}

extern "C" int sprintf (char* buffer, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vsprintf (buffer, fmt, args);
	va_end (args);
	return ret;
}

extern "C" int vswprintf (wchar_t* buffer, size_t bufsiz, const wchar_t* fmt, va_list args)
{
	return CRTL::vsnprintf (buffer, bufsiz, fmt, args);
}

extern "C" int swprintf (wchar_t* buffer, size_t bufsiz, const wchar_t* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vswprintf (buffer, bufsiz, fmt, args);
	va_end (args);
	return ret;
}

extern "C" int _snprintf_s (char* buffer, size_t bufsiz, size_t count, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vsnprintf (buffer, std::min (bufsiz, count + 1), fmt, args);
	va_end (args);
	return ret;
}

extern "C" int vasprintf (char** strp, const char* fmt, va_list arg)
{
	return CRTL::vasprintf (strp, fmt, arg);
}