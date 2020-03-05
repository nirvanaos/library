#ifndef NIRVANA_RUNNABLE_H_
#define NIRVANA_RUNNABLE_H_

#include <CORBA/CORBA.h>

namespace Nirvana {

class Runnable;
typedef ::CORBA::Nirvana::I_ptr <Runnable> Runnable_ptr;
typedef ::CORBA::Nirvana::I_var <Runnable> Runnable_var;
typedef ::CORBA::Nirvana::I_out <Runnable> Runnable_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::Runnable, NIRVANA_REPOSITORY_ID ("Runnable"))
	void (*run) (Bridge <::Nirvana::Runnable>*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Runnable> :
	public T
{
public:
	void run ();
};

template <class T>
void Client <T, ::Nirvana::Runnable>::run ()
{
	Environment _env;
	Bridge < ::Nirvana::Runnable>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.run) (&_b, &_env);
	_env.check ();
}

}
}

namespace Nirvana {

class Runnable : public ::CORBA::Nirvana::ClientInterface <Runnable>
{};

}

#endif
