/// \file
/*
* Nirvana Core.
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
#ifndef NIRVANA_BINDERRORUTL_H_
#define NIRVANA_BINDERRORUTL_H_
#pragma once

#include "Nirvana.h"
#include <Nirvana/BindError.h>

namespace Nirvana {
namespace BindError {

NIRVANA_NORETURN void throw_message (std::string msg);
NIRVANA_NORETURN void throw_invalid_metadata ();
NIRVANA_NORETURN void throw_unsupported_platform (PlatformId platform);
Info& push (Error& err);
void set_message (Info& info, std::string msg);
void set_system (Error& err, const CORBA::SystemException& ex);
void push_obj_name (Error& err, std::string name);

}
}

#endif
