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
#include <locale.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/nls.h>

extern "C" locale_t duplocale (locale_t locobj)
{
  int err = EINVAL;
  try {
    return Nirvana::the_posix->add_locale (Nirvana::the_posix->get_locale (locobj));
  } catch (const CORBA::SystemException& ex) {
    int e = Nirvana::get_minor_errno (ex.minor ());
    if (e)
      err = e;
  } catch (...) {}
  *(int*)Nirvana::the_posix->error_number () = err;
  return 0;
}

extern "C" lconv * __cdecl localeconv ()
{
  return const_cast <lconv*> (Nirvana::the_posix->cur_locale ()->localeconv ());
}
