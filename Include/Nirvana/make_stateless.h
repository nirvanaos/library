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
#ifndef NIRVANA_MAKE_STATELESS_H_
#define NIRVANA_MAKE_STATELESS_H_

#include <CORBA/core_objects.h>
#include <CORBA/ObjectFactory.h>
#include <CORBA/servant_reference.h>
#include <utility>

namespace Nirvana {

template <class T, class ... Args>
CORBA::servant_reference <T> make_stateless (Args ... args)
{
	typename std::aligned_storage <sizeof (T), alignof (T)>::type tmp;
	CORBA::Nirvana::StatelessCreationFrame scb (&tmp, sizeof (T), 0);
	CORBA::Nirvana::g_object_factory->stateless_begin (scb);
	try {
		new (&tmp) T (std::forward <Args> (args)...);
		return CORBA::servant_reference ((T*)CORBA::Nirvana::g_object_factory->stateless_end (true), false);
	} catch (...) {
		CORBA::Nirvana::g_object_factory->stateless_end (false);
		throw;
	}
}

}

#endif
