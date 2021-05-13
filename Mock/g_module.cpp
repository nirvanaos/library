#include <CORBA/Server.h>
#include <Module_s.h>

namespace Nirvana {
namespace Test {

class Module :
	public CORBA::Internal::ServantStatic <Module, ::Nirvana::Module>
{
public:
	static const void* base_address ()
	{
		return nullptr;
	}

	void _add_ref ()
	{}

	void _remove_ref ()
	{}
};

}

extern const ImportInterfaceT <Module> g_module = { OLF_IMPORT_INTERFACE, nullptr, nullptr, NIRVANA_STATIC_BRIDGE (Module, Test::Module) };

}
