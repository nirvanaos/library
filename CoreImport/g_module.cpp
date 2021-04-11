#include <Nirvana/Nirvana.h>
#include <Nirvana/Module.h>
#include <Nirvana/OLF.h>

namespace Nirvana {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <Module> g_module = { OLF_IMPORT_INTERFACE, "Nirvana/g_module", Module::repository_id_ };

}
