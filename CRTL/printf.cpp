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
#include <Nirvana/Formatter.h>
#include <Nirvana/WideIn.h>
#include <Nirvana/WideOut.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/locale_defs.h>
#include "impl/File.h"
#include <limits>

using namespace Nirvana;

namespace CRTL {

class ByteOutFile : public ByteOut
{
public:
	ByteOutFile (File* f) noexcept :
		file_ (f)
	{}

	void put (unsigned c) override;

private:
	File* file_;
};

void ByteOutFile::put (unsigned c)
{
	char ch = (char)c;
	int e = file_->write (&ch, 1);
	if (e)
		throw CORBA::UNKNOWN (make_minor_errno (e));
}

/// \brief Generalized C-style formatting function.
/// As it intended to C formatting, it does not throw exceptions
/// but sets `errno` codes on error instead.
/// 
/// \param fmt Format string.
/// \param args Arguments.
/// \param out Output stream.
/// \param loc Locale conversion settings.
/// \returns Number of wide characters transmitted to the output stream or negative value if an output
///          error or an encoding error (for string and character conversion specifiers) occurred.
int vprintf (WideIn& fmt, va_list args, WideOut& out, const struct lconv* loc) noexcept
{
	int err = EINVAL;
	try {
		return (int)Formatter::format (fmt, args, out, loc);
	} catch (const CORBA::CODESET_INCOMPATIBLE&) {
		err = EILSEQ;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (const std::bad_alloc&) {
		err = ENOMEM;
	} catch (...) {
		err = EINVAL;
	}
	errno = err;
	return -1;
}

/// \brief Generalized vsnprintf implementation
/// 
/// \typeparam C Character type.
/// \param buffer Output buffer pointer.
/// \param bufsz Output buffer size.
/// \param format Format string.
/// \param args Arguments.
/// \returns number of characters (not including the terminating null character) which would have
///   been written to buffer if bufsz was ignored, or a negative value if an encoding error (for
///   string and character conversion specifiers) occurred.
template <class C>
int vsnprintf (C* buffer, size_t bufsz, const C* format, va_list args) noexcept
{
	WideInStrT <C> fmt (format);
	WideOutBufT <C> out (buffer, buffer + bufsz);
	if (vprintf (fmt, args, out, Nirvana::the_posix->cur_locale ()->localeconv ()) >= 0)
		return (int)out.count ();
	else
		return -1;
}

}

extern "C" int vfprintf (FILE* stream, const char* fmt, va_list args)
{
	CRTL::File* file = static_cast <CRTL::File*> (stream);
	auto loc = Nirvana::the_posix->cur_locale ();
	Nirvana::CodePage::_ref_type code_page = Nirvana::CodePage::_downcast (loc->get_facet (LC_CTYPE));
	CRTL::ByteOutFile file_bytes (file);
	Nirvana::WideOutCP out (file_bytes, code_page);
	Nirvana::ByteInStr fmt_bytes (fmt);
	Nirvana::WideInCP in (fmt_bytes, code_page);

	return CRTL::vprintf (in, args, out, loc->localeconv ());
}

extern "C" int fprintf (FILE* stream, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	return vfprintf (stream, fmt, args);
	va_end (args);
}

extern "C" int printf (const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vfprintf (stdout, fmt, args);
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
