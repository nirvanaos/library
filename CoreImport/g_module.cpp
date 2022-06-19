#include <CORBA/CORBA.h>
#include <Nirvana/Module.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <Module> g_module = { OLF_IMPORT_INTERFACE,
"Nirvana/g_module", CORBA::Internal::RepIdOf <Module>::repository_id_ };

}
