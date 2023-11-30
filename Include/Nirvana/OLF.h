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
#pragma once

#include "ImportInterface.h"

#if defined _M_AMD64
#define C_NAME_PREFIX ""
#else
#define C_NAME_PREFIX "_"
#endif

// Instructs MS linker to include symbol
#ifdef _MSC_BUILD
#define NIRVANA_LINK_SYMBOL(s) NIRVANA_PRAGMA (comment (linker, "/include:" C_NAME_PREFIX #s))
#endif

namespace Nirvana {

struct ExportInterface
{
	OLF_Command command;
	const char* name;
	CORBA::Internal::Interface* itf;
};

struct ExportObject
{
	OLF_Command command;
	const char* name;
	CORBA::Internal::Interface* servant_base; // ServantBase*
	CORBA::Internal::Interface* core_object; // ServantBase*
};

struct ExportLocal
{
	OLF_Command command;
	const char* name;
	CORBA::Internal::Interface* local_object; // LocalObject*
	CORBA::Internal::Interface* core_object; // LocalObject*
};

struct ModuleStartup
{
	OLF_Command command;
	CORBA::Internal::Interface* startup;
	uintptr_t flags;
};

const uintptr_t OLF_MODULE_SINGLETON = 1;

}

#if defined (_MSC_BUILD) && !defined (__clang__)

#define NIRVANA_EXPORT(exp, id, I, ...)\
extern "C" NIRVANA_OLF_SECTION constexpr Nirvana::ExportInterface exp {Nirvana::OLF_EXPORT_INTERFACE, id, NIRVANA_STATIC_BRIDGE (I, __VA_ARGS__)};\
NIRVANA_LINK_SYMBOL (exp)

#else

#define NIRVANA_EXPORT(exp, id, I, ...)\
NIRVANA_OLF_SECTION constexpr Nirvana::ExportInterface __attribute__ ((used)) exp{ Nirvana::OLF_EXPORT_INTERFACE, id, NIRVANA_STATIC_BRIDGE (I, __VA_ARGS__) };

#endif

#endif
