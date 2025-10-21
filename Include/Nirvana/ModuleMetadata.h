/// \file
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
#ifndef NIRVANA_MODULEMETADATA_H_
#define NIRVANA_MODULEMETADATA_H_
#pragma once

#include <Nirvana/Nirvana.h>
#include <Nirvana/File.h>
#include <string>
#include <istream>
#include <ostream>

namespace Nirvana {

enum class ModuleType
{
	MODULE_UNKNOWN,
	MODULE_CLASS_LIBRARY,
	MODULE_SINGLETON,
	MODULE_EXECUTABLE,
	MODULE_NIRVANA,
	MODULE_ERROR
};

struct ModuleMetadataEntry
{
	OLF_Command command;
	unsigned flags;
	std::string name;
	std::string interface_id;
};

struct ModuleMetadata
{
	ModuleType type;
	unsigned platform;
	std::vector <ModuleMetadataEntry> entries;
	std::string error;

	ModuleMetadata () :
		type (ModuleType::MODULE_UNKNOWN),
		platform (0)
	{}

	bool check () noexcept;

	void set_error (std::string msg) noexcept
	{
		type = ModuleType::MODULE_ERROR;
		error = std::move (msg);
	}

	void print (std::ostream& out);
};

extern ModuleMetadata get_module_metadata (std::istream& file, bool exe = false);
extern ModuleMetadata get_module_metadata (Nirvana::AccessBuf::_ptr_type file, bool exe = false);

}

#endif
