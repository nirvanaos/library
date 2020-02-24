#ifndef NIRVANA_RUNTIMESUPPORT_H_
#define NIRVANA_RUNTIMESUPPORT_H_

#include <CORBA/Client.h>
#include <CORBA/TypeInterface.h>

namespace Nirvana {

class RuntimeProxy;
typedef ::CORBA::Nirvana::I_ptr <RuntimeProxy> RuntimeProxy_ptr;
typedef ::CORBA::Nirvana::I_var <RuntimeProxy> RuntimeProxy_var;
typedef ::CORBA::Nirvana::I_out <RuntimeProxy> RuntimeProxy_out;

class RuntimeSupport;
typedef ::CORBA::Nirvana::I_ptr <RuntimeSupport> RuntimeSupport_ptr;
typedef ::CORBA::Nirvana::I_var <RuntimeSupport> RuntimeSupport_var;
typedef ::CORBA::Nirvana::I_out <RuntimeSupport> RuntimeSupport_out;

}

namespace CORBA {
namespace Nirvana {

//! \interface RuntimeProxy
//!
//! \brief Used for iterator debugging.
//!		May be used for other purposes.
//!
template <class T>
class Client <T, ::Nirvana::RuntimeProxy> :
	public T
{
public:
	const void* object ();
};

//! \interface RuntimeSupport
//!
//! \brief Run-time library support interface.
//!
template <class T>
class Client <T, ::Nirvana::RuntimeSupport> :
	public T
{
public:
	::Nirvana::RuntimeProxy_var runtime_proxy_get (const void* obj);
	void runtime_proxy_remove (const void* obj);
};

BRIDGE_BEGIN (::Nirvana::RuntimeProxy, NIRVANA_REPOSITORY_ID (RuntimeProxy))
	const void* (*object) (Bridge < ::Nirvana::RuntimeProxy>*, EnvironmentBridge*);
BRIDGE_END ()

BRIDGE_BEGIN (::Nirvana::RuntimeSupport, NIRVANA_REPOSITORY_ID (RuntimeSupport))
	Interface* (*runtime_proxy_get) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, EnvironmentBridge*);
	void (*runtime_proxy_remove) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
const void* Client <T, ::Nirvana::RuntimeProxy>::object ()
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeProxy>& _b (T::_get_bridge (_env));
	const void* _ret = (_b._epv ().epv.object) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::RuntimeProxy_var Client <T, ::Nirvana::RuntimeSupport>::runtime_proxy_get (const void* obj)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	I_ret <::Nirvana::RuntimeProxy> _ret = (_b._epv ().epv.runtime_proxy_get) (&_b, obj, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Nirvana::RuntimeSupport>::runtime_proxy_remove (const void* obj)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.runtime_proxy_remove) (&_b, obj, &_env);
	_env.check ();
}

}
}

namespace Nirvana {

class RuntimeProxy : public ::CORBA::Nirvana::ClientInterface <RuntimeProxy>
{};

class RuntimeSupport : public ::CORBA::Nirvana::ClientInterface <RuntimeSupport>
{};

}

#endif
