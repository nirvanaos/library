#include <CORBA/CORBA.h>
#include <Nirvana/Memory.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {

extern const ImportInterfaceT <Memory> NIRVANA_OLF_SECTION (g_memory) = {OLF_IMPORT_INTERFACE,
"Nirvana/g_memory", CORBA::Internal::RepIdOf <Memory>::id};

}
