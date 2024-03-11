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
#pragma once

#include "NirvanaBase.h"
#include <CORBA/I_ref.h>

#define OLF_BIND "olfbind"

// Stringize _Pragma parameters
#define NIRVANA_PRAGMA(prag) _Pragma (#prag)

#if defined _MSC_VER && !defined (__clang__)

// Lets linker to eliminate unreferenced static structures.
#define NIRVANA_SELECTANY __declspec (selectany)

#pragma section (OLF_BIND, read)

// Instructs compiler and linker to place data into OLF section.
#define NIRVANA_OLF_SECTION __declspec (allocate (OLF_BIND))

#else

// Lets linker to eliminate unreferenced static structures.
#define NIRVANA_SELECTANY __attribute__ ((selectany))

// Instructs compiler and linker to place data into OLF section.
#define NIRVANA_OLF_SECTION __attribute__ ((section (OLF_BIND)))

#endif

/// Instructs compiler and linker to place optional data into OLF section.
#define NIRVANA_OLF_SECTION_OPT NIRVANA_OLF_SECTION NIRVANA_SELECTANY

// We can't use `static const` for import structures with CLang, because it causes the redundant optimization.
// So we use `volatile const`.
#define NIRVANA_STATIC_IMPORT volatile const

namespace Nirvana {

enum OLF_Command : uintptr_t
{
	OLF_END = 0,
	OLF_IMPORT_INTERFACE = 1,
	OLF_EXPORT_INTERFACE = 2,
	OLF_EXPORT_OBJECT = 3,
	OLF_EXPORT_LOCAL = 4,
	OLF_IMPORT_OBJECT = 5,
	OLF_MODULE_STARTUP = 6
};

struct ImportInterface
{
	OLF_Command command;
	const char* name;
	const char* interface_id;
	CORBA::Internal::Interface* itf;
};

template <class I>
struct ImportInterfaceT
{
	ImportInterface imp;

	operator CORBA::Internal::I_ptr <I> () NIRVANA_STATIC_IMPORT noexcept
	{
		assert (imp.itf);
		return reinterpret_cast <I*> (imp.itf);
	}

	I* operator -> () NIRVANA_STATIC_IMPORT noexcept
	{
		assert (imp.itf);
		return reinterpret_cast <I*> (imp.itf);
	}
};

/// Import interface
#define NIRVANA_IMPORT(name, objid, I)\
	NIRVANA_OLF_SECTION_OPT extern NIRVANA_STATIC_IMPORT ::Nirvana::ImportInterfaceT <I> name\
	{::Nirvana::OLF_IMPORT_INTERFACE, objid, ::CORBA::Internal::RepIdOf <I>::id};

}

#endif
