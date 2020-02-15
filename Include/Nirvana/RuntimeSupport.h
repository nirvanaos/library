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

template <>
class Bridge < ::Nirvana::RuntimeProxy> :
	public BridgeMarshal < ::Nirvana::RuntimeProxy>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			const void* (*object) (Bridge < ::Nirvana::RuntimeProxy>*, EnvironmentBridge*);
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
		BridgeMarshal < ::Nirvana::RuntimeProxy> (epv.interface)
	{}
};

template <>
class Bridge < ::Nirvana::RuntimeSupport> :
	public BridgeMarshal < ::Nirvana::RuntimeSupport>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BridgeMarshal < ::Nirvana::RuntimeProxy>* (*runtime_proxy_get) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, EnvironmentBridge*);
			void (*runtime_proxy_remove) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, EnvironmentBridge*);
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
		BridgeMarshal < ::Nirvana::RuntimeSupport> (epv.interface)
	{}
};

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
