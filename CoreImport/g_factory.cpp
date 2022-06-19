#include <CORBA/CORBA.h>
#include <Nirvana/Legacy.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {
namespace Legacy {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <Factory> g_factory = { OLF_IMPORT_INTERFACE,
"Nirvana/Legacy/g_factory", CORBA::Internal::RepIdOf <Factory>::repository_id_ };

}
}
