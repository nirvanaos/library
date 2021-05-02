/// \file
/// Object loadable file structures.
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
#ifndef NIRVANA_OLF_H_
#define NIRVANA_OLF_H_

#include "ImportInterface.h"
#include <CORBA/Object.h>
#include <CORBA/ServantBase.h>
#include <CORBA/LocalObject.h>

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

struct ModuleStartup
{
	OLF_Command command;
	CORBA::Nirvana::Interface* startup;
};

}

#define NIRVANA_EXPORT(exp, id, I, ...)\
extern "C" NIRVANA_OLF_SECTION const Nirvana::ExportInterface exp {Nirvana::OLF_EXPORT_INTERFACE, id, NIRVANA_STATIC_BRIDGE (I, __VA_ARGS__)};\
NIRVANA_LINK_SYMBOL (exp)

#endif
