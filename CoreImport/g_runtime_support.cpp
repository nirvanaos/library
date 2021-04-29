#include <CORBA/CORBA.h>
#include <Nirvana/RuntimeSupport.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <RuntimeSupport> g_runtime_support = { OLF_IMPORT_INTERFACE, "Nirvana/g_runtime_support", RuntimeSupport::repository_id_ };

}
