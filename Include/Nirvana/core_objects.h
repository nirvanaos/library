#ifndef NIRVANA_CORE_OBJECTS_H_
#define NIRVANA_CORE_OBJECTS_H_

#include "HeapFactory.h"
#include "Binder.h"
#include "SynchronizationDomain.h"
#include "OLF.h"

namespace Nirvana {

extern const ImportInterfaceT <Memory> g_memory;
extern const ImportInterfaceT <Binder> g_binder;
extern const ImportInterfaceT <HeapFactory> g_heap_factory;
extern const ImportInterfaceT <SynchronizationDomain> g_sync_domain;

}

#endif
