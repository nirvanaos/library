#include <CORBA/CORBA.h>
#include <Nirvana/Legacy/Legacy.h>
#include <Nirvana/ImportInterface.h>

namespace Nirvana {
namespace Legacy {

extern const ImportInterfaceT <System> NIRVANA_OLF_SECTION (g_system) = { OLF_IMPORT_INTERFACE,
	"Nirvana/Legacy/g_system", CORBA::Internal::RepIdOf <System>::id };

}
}
