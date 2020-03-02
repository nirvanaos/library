#ifndef NIRVANA_SYNCDOMAINTRAITS_S_H_
#define NIRVANA_SYNCDOMAINTRAITS_S_H_

#include "SyncDomainTraits.h"
#include <CORBA/ImplementationPseudo.h>
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::SyncDomainTraits>
{
public:
	static const typename Bridge < ::Nirvana::SyncDomainTraits>::EPV epv_;

protected:
	static ::Nirvana::Pointer _copy_inout (Bridge < ::Nirvana::SyncDomainTraits>* _b, ::Nirvana::ConstPointer src, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).copy_inout (src, size);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ::Nirvana::Pointer _move_out (Bridge < ::Nirvana::SyncDomainTraits>* _b, ::Nirvana::Pointer src, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).move_out (src, size);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _enter (Bridge <::Nirvana::SyncDomainTraits>* _b, Type <::Nirvana::ContextFrame>::ABI_out frame, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).enter (Type <::Nirvana::ContextFrame>::out (frame));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _cxx_leave (Bridge <::Nirvana::SyncDomainTraits>* _b, Type <::Nirvana::ContextFrame>::ABI_in frame, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).leave (Type <::Nirvana::ContextFrame>::in (frame));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _async_call (Bridge <::Nirvana::SyncDomainTraits>* _b, Interface* runnable, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).async_call (TypeI <::Nirvana::Runnable>::in (runnable));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static Type <Boolean>::ABI_ret _get_synchronized (Bridge <::Nirvana::SyncDomainTraits>* _b, EnvironmentBridge* _env)
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
};

template <class S>
const Bridge < ::Nirvana::SyncDomainTraits>::EPV Skeleton <S, ::Nirvana::SyncDomainTraits>::epv_ = {
	{ // header
		Bridge < ::Nirvana::SyncDomainTraits>::interface_id_,
		S::template __duplicate < ::Nirvana::SyncDomainTraits>,
		S::template __release < ::Nirvana::SyncDomainTraits>
	},
	{ // epv
		S::_copy_inout,
		S::_move_out,
		S::_enter,
		S::_cxx_leave,
		S::_async_call,
		S::_get_synchronized
	}
};

/// Standard implementation
template <class S>
class Servant <S, ::Nirvana::SyncDomainTraits> : public ImplementationPseudo <S, ::Nirvana::SyncDomainTraits>
{};

/// Static implementation
template <class S>
class ServantStatic <S, ::Nirvana::SyncDomainTraits> : public ImplementationPseudoStatic <S, ::Nirvana::SyncDomainTraits>
{};

}
}

#endif
