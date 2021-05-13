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
#include <Nirvana/Nirvana.h>
#include <Nirvana/OLF.h>
#include <Nirvana/OLF_Iterator.h>
#include <stdexcept>

namespace Nirvana {

const size_t OLF_Iterator::command_sizes_ [OLF_MODULE_STARTUP] = {
	sizeof (ImportInterface),
	sizeof (ExportInterface),
	sizeof (ExportObject),
	sizeof (ExportLocal),
	sizeof (ImportInterface),
	sizeof (ModuleStartup)
};

void OLF_Iterator::check ()
{
	if (cur_ptr_ >= end_)
		cur_ptr_ = end_;
	else {
		OLF_Command cmd = *cur_ptr_;
		if (OLF_END == cmd)
			cur_ptr_ = end_;
		else if ((size_t)cmd > countof (command_sizes_))
			throw std::runtime_error ("Invalid file format");
	}
}

}
