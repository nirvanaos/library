#ifndef NIRVANA_RUNNABLE_H_
#define NIRVANA_RUNNABLE_H_

#include <CORBA/Interface.h>

namespace Nirvana {

class Runnable;
typedef ::CORBA::Nirvana::I_ptr <Runnable> Runnable_ptr;
typedef ::CORBA::Nirvana::I_var <Runnable> Runnable_var;
typedef ::CORBA::Nirvana::I_out <Runnable> Runnable_out;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge < ::Nirvana::Runnable> :
	public BridgeMarshal < ::Nirvana::Runnable>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			void (*run) (Bridge <::Nirvana::Runnable>*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal < ::Nirvana::Runnable> (epv.interface)
	{}
};

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
