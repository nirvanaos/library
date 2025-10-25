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

// Instructs linker to include symbol
#ifdef _MSC_VER
#define NIRVANA_LINK_SYMBOL(s) NIRVANA_PRAGMA (comment (linker, "/include:" C_NAME_PREFIX #s))
#define NIRVANA_ATTRIBUTE_USED
#else
#define NIRVANA_LINK_SYMBOL(s)
#define NIRVANA_ATTRIBUTE_USED __attribute__ ((used))
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
	const char* name;
	uintptr_t flags;
};

template <typename Word>
struct ModuleStartupW
{
	Word command;
	Word startup;
	Word name;
	Word flags;
};

struct ProcessStartup
{
	uintptr_t command;
	CORBA::Internal::Interface* startup;
};

template <typename Word>
struct ProcessStartupW
{
	Word command;
	Word startup;
};

const uintptr_t OLF_MODULE_SINGLETON = 1; // ModuleStartup::flags

}

#define NIRVANA_EXPORT(exp, id, bridge)\
extern "C" NIRVANA_OLF_SECTION NIRVANA_CONSTINIT const Nirvana::ExportInterface\
	NIRVANA_ATTRIBUTE_USED exp {Nirvana::OLF_EXPORT_INTERFACE, id, bridge };\
	NIRVANA_LINK_SYMBOL (exp)

#define NIRVANA_EXPORT_STATIC(exp, id, ...) NIRVANA_EXPORT (exp, id, (__VA_ARGS__::_bridge ()))
#define NIRVANA_EXPORT_PSEUDO(uname, ...) NIRVANA_EXPORT_STATIC (uname, CORBA::Internal::StaticId <__VA_ARGS__>::id, __VA_ARGS__)

#endif
