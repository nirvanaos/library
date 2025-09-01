/// \file
/// System exception throw helpers.
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
#ifndef NIRVANA_THROW_EXCEPTION_H_
#define NIRVANA_THROW_EXCEPTION_H_
#pragma once

#include "NirvanaBase.h"
#include <CORBA/system_exceptions.h>

#define DECLARE_FUNCTION(e) NIRVANA_NORETURN void throw_##e (uint32_t minor = 0);

namespace Nirvana {

SYSTEM_EXCEPTIONS (DECLARE_FUNCTION)

}

#undef DECLARE_FUNCTION

#include <CORBA/system_exceptions_end.h>

#endif
