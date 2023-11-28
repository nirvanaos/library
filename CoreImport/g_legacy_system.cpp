#include <CORBA/CORBA.h>
#include <Nirvana/Legacy/Legacy.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {
namespace Legacy {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <System> g_system = { OLF_IMPORT_INTERFACE,
	"Nirvana/Legacy/g_system", CORBA::Internal::RepIdOf <System>::id };

}
}
