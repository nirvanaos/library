#include <Nirvana/Memory.h>
#include <Nirvana/OLF.h>

namespace Nirvana {

const ImportInterfaceT <Memory> g_memory = {OLF_IMPORT_INTERFACE, "Nirvana/g_memory", Memory::interface_id_};

}

