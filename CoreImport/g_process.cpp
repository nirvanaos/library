#include <Nirvana/Nirvana.h>

namespace Nirvana {
namespace Legacy {

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <ProcessFactory> g_process = { OLF_IMPORT_INTERFACE,
"Nirvana/Legacy/g_process", ProcessFactory::repository_id_ };

}
}
