#include <CORBA/CORBA.h>
#include <Nirvana/Binder.h>
#include <Nirvana/OLF.h>

namespace Nirvana {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <Binder> g_binder = { OLF_IMPORT_INTERFACE, "Nirvana/g_binder", Binder::repository_id_ };

}
