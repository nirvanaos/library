// This file was generated from "Process.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_PROCESS_H_
#define IDL_PROCESS_H_

namespace Nirvana {
namespace Legacy {

class Process;

}
}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::Legacy::Process> : TypeItf < ::Nirvana::Legacy::Process>
{};

}
}

namespace Nirvana {
namespace Legacy {

#ifdef LEGACY_CORBA_CPP
typedef ::CORBA::Nirvana::Type <Process>::C_ptr Process_ptr;
typedef ::CORBA::Nirvana::Type <Process>::C_var Process_var;
typedef Process_var& Process_out;
#endif

}
}

namespace CORBA {
namespace Nirvana {

template <>
struct Definitions < ::Nirvana::Legacy::Process>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::Legacy::Process, "IDL:Nirvana/Legacy/Process:1.0")
Type <Short>::ABI_ret (*main) (Bridge < ::Nirvana::Legacy::Process>* _b, Type < ::Nirvana::Size>::ABI_in, Type < ::Nirvana::Pointer>::ABI_in, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Legacy::Process> :
	public T,
	public Definitions < ::Nirvana::Legacy::Process>
{
public:
	Type <Short>::Var main (Type < ::Nirvana::Size>::C_in argc, Type < ::Nirvana::Pointer>::C_in argv);
};

template <class T>
Type <Short>::Var Client <T, ::Nirvana::Legacy::Process>::main (Type < ::Nirvana::Size>::C_in argc, Type < ::Nirvana::Pointer>::C_in argv)
{
	Environment _env;
	Bridge < ::Nirvana::Legacy::Process>& _b (T::_get_bridge (_env));
	Type <Short>::C_ret _ret = (_b._epv ().epv.main) (&_b, &argc, &argv, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {
namespace Legacy {

class Process : public ::CORBA::Nirvana::ClientInterface <Process>
{
public:
};

}
}

#endif
