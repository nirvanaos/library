#include <CORBA/Server.h>
#include <IDL/Module_s.h>

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

	void _add_ref () NIRVANA_NOEXCEPT
	{}

	void _remove_ref () NIRVANA_NOEXCEPT
	{}
};

}

NIRVANA_SELECTANY extern
const ImportInterfaceT <Module> g_module = { OLF_IMPORT_INTERFACE, nullptr, nullptr, NIRVANA_STATIC_BRIDGE (Module, Test::Module) };

}
