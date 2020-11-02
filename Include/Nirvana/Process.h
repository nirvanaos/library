/**
~~~{.idl}
module Nirvana {
module Legacy {

/// Process interface
pseudo interface Process
{
	Word run (Size argc, char** argv);
};

}
}
~~~
*/
#ifndef NIRVANA_LEGACY_PROCESS_H_
#define NIRVANA_LEGACY_PROCESS_H_

#include <CORBA/Client.h>

namespace Nirvana {
namespace Legacy {

class Process;
typedef ::CORBA::Nirvana::I_ptr <Process> Process_ptr;
typedef ::CORBA::Nirvana::I_var <Process> Process_var;
typedef ::CORBA::Nirvana::I_out <Process> Process_out;

}
}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::Legacy::Process, NIRVANA_REPOSITORY_ID ("Legacy/Process"))
::Nirvana::Word (*run) (Bridge <::Nirvana::Legacy::Process>*, ::Nirvana::Size argc, Char** argv, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Legacy::Process> :
	public T
{
public:
	::Nirvana::Word run (::Nirvana::Size argc, Char** argv);
};

template <class T>
::Nirvana::Word Client <T, ::Nirvana::Legacy::Process>::run (::Nirvana::Size argc, Char** argv)
{
	Environment _env;
	Bridge < ::Nirvana::Legacy::Process>& _b (T::_get_bridge (_env));
	Long _ret = (_b._epv ().epv.run) (&_b, argc, argv, &_env);
	_env.check ();
	return _ret;
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
