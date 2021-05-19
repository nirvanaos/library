#include <CORBA/CORBA.h>
#include <Nirvana/System.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <System> g_system = { OLF_IMPORT_INTERFACE, "Nirvana/g_system", System::repository_id_ };

}
