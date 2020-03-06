#ifndef NIRVANA_SYNCHRONIZATIONCONTEXT_S_H_
#define NIRVANA_SYNCHRONIZATIONCONTEXT_S_H_

#include "SynchronizationContext.h"
#include <CORBA/ImplementationPseudo.h>
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::SynchronizationContext>
{
public:
	static const typename Bridge < ::Nirvana::SynchronizationContext>::EPV epv_;

protected:
	static void _enter_memory (Bridge < ::Nirvana::SynchronizationContext>* _b, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).enter_memory ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _enter (Bridge <::Nirvana::SynchronizationContext>* _b, Type <Boolean>::ABI_in ret, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).enter (Type <Boolean>::in (ret));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static ::Nirvana::Pointer _adopt_output (Bridge < ::Nirvana::SynchronizationContext>* _b, ::Nirvana::Pointer src, ::Nirvana::UWord* size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).adopt_output (src, Type <::Nirvana::UWord>::inout (size));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ::Nirvana::Pointer _allocate (Bridge < ::Nirvana::SynchronizationContext>* _b, ::Nirvana::UWord* size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).allocate (Type <::Nirvana::UWord>::inout (size));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _async_call (Bridge <::Nirvana::SynchronizationContext>* _b, Interface* runnable, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).async_call (TypeI <::Nirvana::Runnable>::in (runnable));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static Type <Boolean>::ABI_ret _get_synchronized (Bridge <::Nirvana::SynchronizationContext>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).synchronized ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Type <Boolean>::ABI_ret _shared_memory (Bridge <::Nirvana::SynchronizationContext>* _b, Interface* other, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).shared_memory (TypeI <::Nirvana::SynchronizationContext>::in (other));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::SynchronizationContext>::EPV Skeleton <S, ::Nirvana::SynchronizationContext>::epv_ = {
	{ // header
		Bridge < ::Nirvana::SynchronizationContext>::interface_id_,
		S::template __duplicate < ::Nirvana::SynchronizationContext>,
		S::template __release < ::Nirvana::SynchronizationContext>
	},
	{ // epv
		S::_enter_memory,
		S::_enter,
		S::_adopt_output,
		S::_allocate,
		S::_async_call,
		S::_get_synchronized,
		S::_shared_memory
	}
};

/// Standard implementation
template <class S>
class Servant <S, ::Nirvana::SynchronizationContext> : public ImplementationPseudo <S, ::Nirvana::SynchronizationContext>
{};

/// Static implementation
template <class S>
class ServantStatic <S, ::Nirvana::SynchronizationContext> : public ImplementationPseudoStatic <S, ::Nirvana::SynchronizationContext>
{};

}
}

#endif
