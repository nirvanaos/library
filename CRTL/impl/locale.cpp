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
#include "../pch/pch.h"
#include "locale.h"
#include <Nirvana/POSIX.h>

struct EPV
{
	const char* interface_id;
};

struct __Locale
{
	const EPV* epv;
};

namespace CRTL {

Nirvana::Locale::_ptr_type check_locale (locale_t locobj) noexcept
{
	Nirvana::Locale::_ptr_type ret = nullptr;
	if (LC_GLOBAL_LOCALE == locobj) {
		try {
			ret = Nirvana::the_posix->cur_locale ();
		} catch (...) {}
	} else if (locobj && CORBA::Internal::RepId::compatible (locobj->epv->interface_id,
		CORBA::Internal::RepIdOf <Nirvana::Locale>::id))
		ret = reinterpret_cast <Nirvana::Locale*> (locobj);

	if (!ret)
		errno = EINVAL;
	return ret;
}

Nirvana::CodePage::_ref_type get_cp (locale_t l) noexcept
{
	Nirvana::Locale::_ptr_type loc = check_locale (l);
	if (!loc)
		return nullptr;
	try {
		return Nirvana::CodePage::_downcast (loc->get_facet (LC_CTYPE));
	} catch (...) {
		return nullptr;
	}
}

Nirvana::CodePage::_ptr_type cur_code_page () noexcept
{
	try {
		return Nirvana::CodePage::_downcast (Nirvana::the_posix->cur_locale ()->get_facet (LC_CTYPE));
	} catch (...) {
		return nullptr;
	}
}

}
