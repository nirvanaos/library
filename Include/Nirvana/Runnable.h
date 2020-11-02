#ifndef NIRVANA_LEGACY_RUNNABLE_H_
#define NIRVANA_LEGACY_RUNNABLE_H_

#include <CORBA/CORBA.h>

namespace Nirvana {
namespace Legacy {

class Runnable;
typedef ::CORBA::Nirvana::I_ptr <Runnable> Runnable_ptr;
typedef ::CORBA::Nirvana::I_var <Runnable> Runnable_var;
typedef ::CORBA::Nirvana::I_out <Runnable> Runnable_out;

}
}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::Legacy::Runnable, NIRVANA_REPOSITORY_ID ("Legacy/Runnable"))
	void (*run) (Bridge <::Nirvana::Legacy::Runnable>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Legacy::Runnable> :
	public T
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
{};

}
}

#endif
