#include <CORBA/CORBA.h>
#include <Nirvana/Chrono.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <Chrono> g_chrono = { OLF_IMPORT_INTERFACE, "Nirvana/g_chrono", Chrono::repository_id_ };

}
