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
#ifndef NIRVANA_CORE_OBJECTS_H_
#define NIRVANA_CORE_OBJECTS_H_

#include "ImportInterface.h"

namespace Nirvana {

class Memory;
class Binder;
class HeapFactory;
class Current;
class RuntimeSupport;
class Module;
class Chrono;

extern const ImportInterfaceT <Memory> g_memory;
extern const ImportInterfaceT <Binder> g_binder;
extern const ImportInterfaceT <HeapFactory> g_heap_factory;
extern const ImportInterfaceT <Current> g_current;
extern const ImportInterfaceT <RuntimeSupport> g_runtime_support;
extern const ImportInterfaceT <Module> g_module;
extern const ImportInterfaceT <Chrono> g_chrono;

}

#endif
