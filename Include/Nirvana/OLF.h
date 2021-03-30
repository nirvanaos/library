/// \file
/// Object loadable file structures.

/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_OLF_H_
#define NIRVANA_OLF_H_

#include "ImportInterface.h"
#include <CORBA/ServantBase.h>
#include <CORBA/LocalObject.h>

#define OLF_BIND "olfbind"

#pragma section (OLF_BIND, read, execute)

/// Instructs compiler and linker to place data into OLF section.
#define NIRVANA_OLF_SECTION __declspec (allocate (OLF_BIND))

#if defined _M_AMD64
#define C_NAME_PREFIX ""
#else
#define C_NAME_PREFIX "_"
#endif

/// Instructs linker to include symbol
#define NIRVANA_LINK_SYMBOL(s) __pragma(comment (linker, "/include:" C_NAME_PREFIX #s))

namespace Nirvana {

struct ExportInterface
{
	OLF_Command command;
	const char* name;
	CORBA::Nirvana::Interface* itf;
};

struct ExportObject
{
	OLF_Command command;
	const char* name;
	CORBA::Nirvana::Interface* servant_base;
	CORBA::Nirvana::Bridge <::PortableServer::ServantBase>* core_object;
};

struct ExportLocal
{
	OLF_Command command;
	const char* name;
	CORBA::Nirvana::Interface* local_object;
	CORBA::Nirvana::Interface* abstract_base;
	CORBA::Nirvana::Bridge <CORBA::LocalObject>* core_object;
};

template <class S>
struct StaticId
{
	static const char static_id_ [];
};

template <class S> struct PrimaryInterface;

template <class S, class I = typename PrimaryInterface <S>::Itf>
class Static
{
public:
	static CORBA::Nirvana::I_ptr <I> ptr ()
	{
		assert (import_.itf);
		return (I*)import_.itf;
	}

private:
	// We can't use `static const` here, because it causes the redundant optimization in CLang.
	NIRVANA_OLF_SECTION static volatile ImportInterface import_;
};

template <class S, class I>
NIRVANA_OLF_SECTION volatile ImportInterface Static <S, I>::import_{ OLF_IMPORT_OBJECT, StaticId <S>::static_id_, I::repository_id_ };

}

#define NIRVANA_EXPORT(exp, id, I, ...)\
extern "C" NIRVANA_OLF_SECTION const Nirvana::ExportInterface exp {Nirvana::OLF_EXPORT_INTERFACE, id, STATIC_BRIDGE (I, __VA_ARGS__)};\
NIRVANA_LINK_SYMBOL (exp)

#endif
