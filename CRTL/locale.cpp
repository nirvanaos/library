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
#include <CORBA/I_var.h>

extern "C" locale_t duplocale (locale_t locobj)
{
  try {
    Nirvana::Locale::_ptr_type src = (LC_GLOBAL_LOCALE == locobj) ?
      Nirvana::the_posix->cur_locale () : Nirvana::Locale::_check ((CORBA::Internal::Interface*)locobj);
    return CORBA::Internal::interface_duplicate (&src);
  } catch (...) {
  }
  *(int*)Nirvana::the_posix->error_number () = EINVAL;
  return nullptr;
}

extern "C" void freelocale (locale_t locobj)
{
  CORBA::Internal::interface_release ((CORBA::Internal::Interface*)locobj);
}

extern "C" lconv* localeconv (void)
{
  return const_cast <lconv*> (Nirvana::the_posix->cur_locale ()->localeconv ());
}

extern "C" locale_t newlocale (int category, const char* locale, locale_t base)
{
  int err = EINVAL;
  try {
    Nirvana::Locale::_ptr_type loc_base = Nirvana::Locale::_check ((CORBA::Internal::Interface*)base);
    CORBA::Internal::I_var <Nirvana::Locale> new_loc = Nirvana::the_posix->create_locale (category, locale, loc_base);
    CORBA::Internal::interface_release (&loc_base);
    return &new_loc._retn ();
  } catch (const CORBA::SystemException& ex) {
    int e = Nirvana::get_minor_errno (ex.minor ());
    if (e)
      err = e;
  } catch (...) {
  }
  *(int*)Nirvana::the_posix->error_number () = err;
  return nullptr;
}

extern "C" char* setlocale (int category, const char* locale)
{
  int err = EINVAL;
  try {
    Nirvana::the_posix->cur_locale (
      Nirvana::the_posix->create_locale (category, locale, Nirvana::the_posix->cur_locale ()));
    return const_cast <char*> (locale);
  } catch (const CORBA::SystemException& ex) {
    int e = Nirvana::get_minor_errno (ex.minor ());
    if (e)
      err = e;
  } catch (...) {
  }
  *(int*)Nirvana::the_posix->error_number () = err;
  return nullptr;
}

extern "C" locale_t uselocale (locale_t loc)
{
  Nirvana::Locale::_ptr_type ret = Nirvana::the_posix->cur_locale ();
  if (loc && LC_GLOBAL_LOCALE != loc) {
    int err = EINVAL;
    try {
      Nirvana::the_posix->cur_locale (Nirvana::Locale::_check ((CORBA::Internal::Interface*)loc));
    } catch (const CORBA::SystemException& ex) {
      int e = Nirvana::get_minor_errno (ex.minor ());
      if (e)
        err = e;
    } catch (...) {
    }
    *(int*)Nirvana::the_posix->error_number () = err;
    return nullptr;
  }
  return &ret;
}
