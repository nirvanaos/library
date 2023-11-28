#include <CORBA/Server.h>
#include <Nirvana/Module_s.h>

namespace Nirvana {
namespace Test {

class Module :
	public CORBA::servant_traits <Nirvana::Module>::ServantStatic <Module>
{
public:
	static const void* base_address ()
	{
		return nullptr;
	}

	static void _add_ref () noexcept
	{}

	static void _remove_ref () noexcept
	{}

	static void atexit (AtExitFunc f)
	{
		::atexit (f);
	}
};

}

NIRVANA_SELECTANY extern
const ImportInterfaceT <Module> g_module = { OLF_IMPORT_INTERFACE, nullptr, nullptr, NIRVANA_STATIC_BRIDGE (Module, Test::Module) };

}
