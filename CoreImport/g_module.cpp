#include <CORBA/CORBA.h>
#include <Nirvana/Module.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {

extern const ImportInterfaceT <Module> NIRVANA_OLF_SECTION (g_module) = { OLF_IMPORT_INTERFACE,
"Nirvana/g_module", CORBA::Internal::RepIdOf <Module>::id };

}
