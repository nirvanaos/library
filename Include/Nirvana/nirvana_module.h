/// \file
/// \brief This file must be included once into one of the Nirvana module sources.
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
#ifndef NIRVANA_NIRVANA_MODULE_H_
#define NIRVANA_NIRVANA_MODULE_H_
#pragma once

#include "ModuleInitImpl.h"

#define NIRVANA_MODULE_EX(name, flags) NIRVANA_EXPORT (Nirvana::ModuleStartup, _entry_point,\
	Nirvana::OLF_MODULE_STARTUP, Nirvana::ModuleInitImpl::_bridge (), name, flags)

#define NIRVANA_MODULE(name) NIRVANA_MODULE_EX (name, 0)
#define NIRVANA_SINGLETON(name) NIRVANA_MODULE_EX (name, Nirvana::OLF_MODULE_SINGLETON)

#endif
