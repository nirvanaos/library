#ifndef NIRVANA_RUNTIMESUPPORT_S_H_
#define NIRVANA_RUNTIMESUPPORT_S_H_

#include "RuntimeSupport.h"
#include <CORBA/ImplementationPseudo.h>

namespace CORBA {
namespace Nirvana {

// RuntimeProxy

template <class S>
class Skeleton <S, ::Nirvana::RuntimeProxy>
{
public:
	static const typename Bridge < ::Nirvana::RuntimeProxy>::EPV epv_;

protected:
	static const void* _object (Bridge < ::Nirvana::RuntimeProxy>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).object ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::RuntimeProxy>::EPV Skeleton <S, ::Nirvana::RuntimeProxy>::epv_ = {
	{ // interface
		Bridge < ::Nirvana::RuntimeProxy>::interface_id_,
		S::template __duplicate < ::Nirvana::RuntimeProxy>,
		S::template __release < ::Nirvana::RuntimeProxy>
	},
	{ // epv
		S::_object
	}
};

// RuntimeSupport

template <class S>
class Skeleton <S, ::Nirvana::RuntimeSupport>
{
public:
	static const typename Bridge < ::Nirvana::RuntimeSupport>::EPV epv_;

protected:
	static Interface* _runtime_proxy_get (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* obj, EnvironmentBridge* _env)
	{
		try {
			return TypeI < ::Nirvana::RuntimeProxy>::ret (S::_implementation (_b).runtime_proxy_get (obj));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _runtime_proxy_remove (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* key, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).runtime_proxy_remove (key);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
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
		S::_runtime_proxy_get,
		S::_runtime_proxy_remove
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
