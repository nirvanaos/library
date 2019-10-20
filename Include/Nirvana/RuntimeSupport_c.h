#ifndef NIRVANA_RUNTIMESUPPORT_C_H_
#define NIRVANA_RUNTIMESUPPORT_C_H_

#include <CORBA/Interface_c.h>
#include "NirvanaBase.h"

namespace Nirvana {

class RuntimeSupport;
typedef ::CORBA::Nirvana::T_ptr <RuntimeSupport> RuntimeSupport_ptr;
typedef ::CORBA::Nirvana::T_var <RuntimeSupport> RuntimeSupport_var;
typedef ::CORBA::Nirvana::T_out <RuntimeSupport> RuntimeSupport_out;

}

namespace CORBA {
namespace Nirvana {

template <class T>
class Client <T, ::Nirvana::RuntimeSupport> :
	public T
{
public:
	Boolean object_set_add (const void* key);
	Boolean object_set_remove (const void* key);
	Boolean object_set_check (const void* key);

	void shared_object_set (const void* key, Interface_ptr obj);
	Boolean shared_object_remove (const void* key);
	Interface_ptr shared_object_get (const void* key);
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
			Boolean (*object_set_add) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, EnvironmentBridge*);
			Boolean (*object_set_remove) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, EnvironmentBridge*);
			Boolean (*object_set_check) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, EnvironmentBridge*);
			void (*shared_object_set) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, BridgeMarshal <Interface>*, EnvironmentBridge*);
			Boolean (*shared_object_remove) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, EnvironmentBridge*);
			BridgeMarshal <Interface>* (*shared_object_get) (Bridge < ::Nirvana::RuntimeSupport>*, const void*, EnvironmentBridge*);
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
Boolean Client <T, ::Nirvana::RuntimeSupport>::object_set_add (const void* key)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.object_set_add) (&_b, key, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::Nirvana::RuntimeSupport>::object_set_remove (const void* key)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.object_set_remove) (&_b, key, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::Nirvana::RuntimeSupport>::object_set_check (const void* key)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.object_set_check) (&_b, key, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Nirvana::RuntimeSupport>::shared_object_set (const void* key, Interface_ptr obj)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.shared_object_set) (&_b, key, obj, &_env);
	_env.check ();
}

template <class T>
Boolean Client <T, ::Nirvana::RuntimeSupport>::shared_object_remove (const void* key)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.shared_object_remove) (&_b, key, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Interface_ptr Client <T, ::Nirvana::RuntimeSupport>::shared_object_get (const void* key)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	Interface_var _ret = (_b._epv ().epv.shared_object_get) (&_b, key, &_env);
	_env.check ();
	return _ret._retn ();
}

}
}

namespace Nirvana {

class RuntimeSupport : public ::CORBA::Nirvana::ClientInterface <RuntimeSupport>
{};

extern ::CORBA::Nirvana::Bridge <RuntimeSupport>* const g_runtime_support;

static RuntimeSupport_ptr runtime_support ()
{
	return static_cast <RuntimeSupport*> (g_runtime_support);
}

}

#endif
