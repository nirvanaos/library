#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>

namespace Nirvana {

extern const ImportInterfaceT <System> NIRVANA_OLF_SECTION (g_system) = { OLF_IMPORT_INTERFACE,
"Nirvana/g_system", CORBA::Internal::RepIdOf <System>::id };

}
