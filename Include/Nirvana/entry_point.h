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
#ifndef NIRVANA_ENTRY_POINT_H_
#define NIRVANA_ENTRY_POINT_H_
#pragma once

#if defined (NIRVANA_MODULE) || defined (NIRVANA_SINGLETON) || defined (NIRVANA_PROCESS)

#if defined (NIRVANA_PROCESS)
#include "ProcessMain.h"
#else
#include "ModuleInitImpl.h"
#endif

extern "C" NIRVANA_OLF_SECTION_OPT const Nirvana::ModuleStartup
#ifndef _MSC_VER
__attribute__ ((used))
#endif
entry_point { Nirvana::OLF_MODULE_STARTUP, 
#ifndef NIRVANA_PROCESS
  Nirvana::ModuleInitImpl::_bridge (),
#ifdef NIRVANA_SINGLETON
  Nirvana::OLF_MODULE_SINGLETON
#else
  0
#endif
#else
  Nirvana::ProcessMain::_bridge (), 0
#endif
};

#ifdef _MSC_VER
NIRVANA_LINK_SYMBOL (entry_point);
#endif

#endif

#endif
