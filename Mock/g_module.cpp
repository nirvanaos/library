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

	template <class I>
	static CORBA::Internal::Interface* __duplicate (CORBA::Internal::Interface* itf, CORBA::Internal::Interface* env) noexcept
	{
		return itf;
	}

	template <class I>
	static void __release (CORBA::Internal::Interface*) noexcept
	{
	}

	static void atexit (AtExitFunc f)
	{
		::atexit (f);
	}
};

}

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <Module> g_module = { OLF_IMPORT_INTERFACE, nullptr, nullptr, NIRVANA_STATIC_BRIDGE (Module, Test::Module) };

}
