#include <CORBA/CORBA.h>
#include <Nirvana/Memory.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <Memory> g_memory = {OLF_IMPORT_INTERFACE, "Nirvana/g_memory", Memory::repository_id_};

}
