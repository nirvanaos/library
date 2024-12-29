#include <CORBA/Corba.h>
#include <Mock/MockMemory.h>

namespace Nirvana {
namespace Test {

extern NIRVANA_MOCK_IMPORT CORBA::Internal::Interface* mock_debugger;
extern NIRVANA_MOCK_IMPORT CORBA::Internal::Interface* mock_memory;
extern NIRVANA_MOCK_IMPORT CORBA::Internal::Interface* mock_module;

}

#ifdef _MSC_VER
#pragma warning(disable:4073)
#pragma init_seg(lib)
#endif

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <Debugger> the_debugger = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, Test::mock_debugger };

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <Memory> the_memory = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, Test::mock_memory };

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <Module> the_module = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, Test::mock_module };

}
