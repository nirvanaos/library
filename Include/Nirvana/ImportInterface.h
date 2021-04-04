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
#ifndef NIRVANA_IMPORTINTERFACE_H_
#define NIRVANA_IMPORTINTERFACE_H_

#include "NirvanaBase.h"
#include <CORBA/I_ptr.h>

namespace Nirvana {

enum OLF_Command : uintptr_t
{
	OLF_END = 0,
	OLF_IMPORT_INTERFACE = 1,
	OLF_EXPORT_INTERFACE = 2,
	OLF_EXPORT_OBJECT = 3,
	OLF_EXPORT_LOCAL = 4,
	OLF_IMPORT_OBJECT = 5
};

struct ImportInterface
{
	OLF_Command command;
	const char* name;
	const char* interface_id;
	CORBA::Nirvana::Interface* itf;
};

template <class I>
struct ImportInterfaceT
{
	ImportInterface imp;

	operator CORBA::Nirvana::I_ptr <I> () const
	{
		assert (imp.itf);
		return static_cast <I*> (imp.itf);
	}

	I* operator -> () const
	{
		assert (imp.itf);
		return static_cast <I*> (imp.itf);
	}
};

}

#endif
