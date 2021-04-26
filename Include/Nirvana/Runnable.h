// This file was generated from "Runnable.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_RUNNABLE_H_
#define IDL_RUNNABLE_H_

namespace Nirvana {
namespace Legacy {

class Runnable;

}
}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::Legacy::Runnable> : TypeItf < ::Nirvana::Legacy::Runnable>
{};

}
}

namespace Nirvana {
namespace Legacy {

#ifdef LEGACY_CORBA_CPP
typedef ::CORBA::Nirvana::Type <Runnable>::C_ptr Runnable_ptr;
typedef ::CORBA::Nirvana::Type <Runnable>::C_var Runnable_var;
typedef Runnable_var& Runnable_out;
#endif

}
}

namespace CORBA {
namespace Nirvana {

template <>
struct Definitions < ::Nirvana::Legacy::Runnable>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::Legacy::Runnable, "IDL:Nirvana/Legacy/Runnable:1.0")
void (*run) (Bridge < ::Nirvana::Legacy::Runnable>* _b, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Legacy::Runnable> :
	public T,
	public Definitions < ::Nirvana::Legacy::Runnable>
{
public:
	void run ();
};

template <class T>
void Client <T, ::Nirvana::Legacy::Runnable>::run ()
{
	Environment _env;
	Bridge < ::Nirvana::Legacy::Runnable>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.run) (&_b, &_env);
	_env.check ();
}

}
}

namespace Nirvana {
namespace Legacy {

class Runnable : public ::CORBA::Nirvana::ClientInterface <Runnable>
{
public:
};

}
}

#endif
