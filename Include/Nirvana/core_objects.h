#ifndef NIRVANA_CORE_OBJECTS_H_
#define NIRVANA_CORE_OBJECTS_H_

#include "ImportInterface.h"
#include "HeapFactory.h"
#include "Binder.h"
#include "Current.h"
#include "RuntimeSupport.h"
#include "Module.h"

namespace Nirvana {

extern const ImportInterfaceT <Memory> g_memory;
extern const ImportInterfaceT <Binder> g_binder;
extern const ImportInterfaceT <HeapFactory> g_heap_factory;
extern const ImportInterfaceT <Current> g_current;
extern const ImportInterfaceT <RuntimeSupport> g_runtime_support;
extern const ImportInterfaceT <Module> g_module;

}

#endif
