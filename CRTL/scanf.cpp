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
#include <Nirvana/Parser.h>
#include <Nirvana/WideIn.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/locale_defs.h>
#include "impl/File.h"

using namespace Nirvana;

namespace CRTL {

/// @brief Input from a file stream.
class ByteInFile : public ByteIn
{
public:
	ByteInFile (CRTL::File* f) noexcept :
		file_ (f)
	{}

	int get () override;

private:
	CRTL::File* file_;
};

int ByteInFile::get ()
{
	char ch;
	size_t cb;
	int e = file_->read (&ch, 1, cb);
	if (e)
		throw CORBA::UNKNOWN (make_minor_errno (e));
	if (!cb)
		return EOF;
	return ch;
}

/// \brief Generalized C-style scan function.
/// As it intended to C, it does not throw exceptions
/// but sets `errno` codes on error instead.
int vscanf (WideIn& in, WideIn& fmt, va_list args, const struct lconv* loc = nullptr)
{
	size_t cnt = -1;
	try {
		Parser::parse (in, fmt, args, cnt, loc);
		errno = 0;
	} catch (const CORBA::SystemException& ex) {
		int err = get_minor_errno (ex.minor ());
		if (!err)
			err = EINVAL;
		errno = err;
	} catch (...) {
		errno = EINVAL;
	}
	return (int)cnt;
}

/// \brief Generalized C-style scan function.
/// As it intended to C, it does not throw exceptions
/// but sets `errno` codes on error instead.
template <class C>
int vsscanf (const C* buffer, const C* fmt, va_list args, const struct lconv* loc = nullptr)
{
	WideInStrT <C> in (buffer);
	WideInStrT <C> infmt (fmt);
	return vscanf (in, infmt, args, loc);
}

}

using namespace CRTL;

extern "C" int vfscanf (FILE* stream, const char* fmt, va_list args)
{
	CRTL::File* file = static_cast <CRTL::File*> (stream);
	auto loc = Nirvana::the_posix->cur_locale ();
	CodePage::_ref_type code_page = CodePage::_downcast (loc->get_facet (LC_CTYPE));
	CRTL::ByteInFile file_bytes (file);
	WideInCP in (file_bytes, code_page);
	WideInStrUTF8 infmt (fmt);

	return CRTL::vscanf (in, infmt, args, loc->localeconv ());
}

extern "C" int vscanf (const char* fmt, va_list args)
{
	return vfscanf (stdin, fmt, args);
}

extern "C" int fscanf (FILE* stream, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vfscanf (stream, fmt, args);
	va_end (args);
	return ret;
}

extern "C" int scanf (const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vscanf (fmt, args);
	va_end (args);
	return ret;
}

extern "C" int vsscanf (const char* str, const char* fmt, va_list args)
{
	return CRTL::vsscanf (str, fmt, args, Nirvana::the_posix->cur_locale ()->localeconv ());
}

extern "C" int sscanf (const char* str, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	int ret = vsscanf (str, fmt, args);
	va_end (args);
	return ret;
}
/*
extern "C" int sscanf_l (const char* restrict, locale_t, const char* restrict, ...)
{

}*/

