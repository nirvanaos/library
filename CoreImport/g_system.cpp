#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>

namespace Nirvana {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <System> g_system = { OLF_IMPORT_INTERFACE,
"Nirvana/g_system", CORBA::Internal::RepIdOf <System>::id };

}
