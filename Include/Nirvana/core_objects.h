#ifndef NIRVANA_CORE_OBJECTS_H_
#define NIRVANA_CORE_OBJECTS_H_

#include "HeapFactory.h"
#include "Binder.h"
#include "SynchronizationDomain.h"
#include <CORBA/ObjectFactory.h>
#include <CORBA/LocalMarshal.h>
#include "RuntimeSupport.h"
#include "OLF.h"

namespace Nirvana {

extern const ImportInterfaceT <Memory> g_memory;
extern const ImportInterfaceT <Binder> g_binder;
extern const ImportInterfaceT <HeapFactory> g_heap_factory;
extern const ImportInterfaceT <SynchronizationDomain> g_sync_domain;
extern const ImportInterfaceT <RuntimeSupport> g_runtime_support;
}

namespace CORBA {
namespace Nirvana {

extern const ::Nirvana::ImportInterfaceT <ObjectFactory> g_object_factory;
extern const ::Nirvana::ImportInterfaceT <LocalMarshal> g_local_marshal;

}
}

#endif
