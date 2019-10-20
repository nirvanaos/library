#ifndef NIRVANA_RUNTIMESUPPORT_S_H_
#define NIRVANA_RUNTIMESUPPORT_S_H_

#include "RuntimeSupport_c.h"
#include <CORBA/Servant.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::RuntimeSupport>
{
public:
	static const typename Bridge < ::Nirvana::RuntimeSupport>::EPV epv_;

protected:
	static Boolean _object_set_add (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* key, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).object_set_add (key);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean _object_set_remove (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* key, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).object_set_remove (key);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean _object_set_check (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* key, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).object_set_check (key);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static void _shared_object_set (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* key, BridgeMarshal <Interface>* obj, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).shared_object_set (key, _unmarshal_in (obj));
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
	}

	static Boolean _shared_object_remove (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* key, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).shared_object_remove (key);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static BridgeMarshal <Interface>* _shared_object_get (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* key, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).shared_object_get (key);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::RuntimeSupport>::EPV Skeleton <S, ::Nirvana::RuntimeSupport>::epv_ = {
	{ // interface
		Bridge < ::Nirvana::RuntimeSupport>::interface_id_,
		S::template __duplicate < ::Nirvana::RuntimeSupport>,
		S::template __release < ::Nirvana::RuntimeSupport>
	},
	{ // epv
		S::_object_set_add,
		S::_object_set_remove,
		S::_object_set_check,
		S::_shared_object_set,
		S::_shared_object_remove,
		S::_shared_object_get
	}
};


// Standard implementation

template <class S>
class Servant <S, ::Nirvana::RuntimeSupport> : public ImplementationPseudo <S, ::Nirvana::RuntimeSupport>
{};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::RuntimeSupport> : public ImplementationStaticPseudo <S, ::Nirvana::RuntimeSupport>
{};

}
}

#endif
