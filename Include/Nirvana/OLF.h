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
#ifdef _MSC_VER
#define NIRVANA_LINK_SYMBOL(s) NIRVANA_PRAGMA (comment (linker, "/include:" C_NAME_PREFIX #s))
#endif

namespace Nirvana {

struct ExportInterface
{
	uintptr_t command;
	const char* name;
	CORBA::Internal::Interface* itf;
};

template <typename Word>
struct ExportInterfaceW
{
	Word command;
	Word name;
	Word itf;
};

struct ExportObject
{
	uintptr_t command;
	const char* name;
	CORBA::Internal::Interface* servant;
	CORBA::Internal::Interface* core_object;
};

template <typename Word>
struct ExportObjectW
{
	Word command;
	Word name;
	Word servant;
	Word core_object;
};

struct ModuleStartup
{
	uintptr_t command;
	CORBA::Internal::Interface* startup;
	uintptr_t flags;
};

template <typename Word>
struct ModuleStartupW
{
	Word command;
	Word startup;
	Word flags;
};

const uintptr_t OLF_MODULE_SINGLETON = 1; // ModuleStartup::flags

}

#if !defined (NIRVANA_PROCESS) && !defined (NIRVANA_SINGLETON)

#if defined (_MSC_VER) && !defined (__clang__)

#define NIRVANA_EXPORT(exp, id, Itf, ...)\
extern "C" NIRVANA_OLF_SECTION const Nirvana::ExportInterface exp {Nirvana::OLF_EXPORT_INTERFACE, id, NIRVANA_STATIC_BRIDGE (Itf, __VA_ARGS__)};\
NIRVANA_LINK_SYMBOL (exp)

#else

#define NIRVANA_EXPORT(exp, id, Itf, ...)\
NIRVANA_OLF_SECTION const Nirvana::ExportInterface __attribute__ ((used)) exp{ Nirvana::OLF_EXPORT_INTERFACE, id, NIRVANA_STATIC_BRIDGE (Itf, __VA_ARGS__) };

#endif

#define NIRVANA_EXPORT_PSEUDO(uname, Impl) NIRVANA_EXPORT (uname, CORBA::Internal::StaticId <Impl>::id, Impl::PrimaryInterface, Impl)

#endif

#endif
