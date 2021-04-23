// This file was generated from "RuntimeSupport.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_RUNTIMESUPPORT_S_H_
#define IDL_RUNTIMESUPPORT_S_H_
#include "RuntimeSupport.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::RuntimeProxy>
{
public:
	static const typename Bridge < ::Nirvana::RuntimeProxy>::EPV epv_;

protected:
	static Type < ::Nirvana::ConstPointer>::ABI_ret _object (Bridge < ::Nirvana::RuntimeProxy>* _b, Interface* _env)
	{
		try {
			return Type < ::Nirvana::ConstPointer>::ret (S::_implementation (_b).object ());
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type < ::Nirvana::ConstPointer>::ret ();
	}

};

template <class S>
const Bridge < ::Nirvana::RuntimeProxy>::EPV Skeleton <S, ::Nirvana::RuntimeProxy>::epv_ = {
	{ // header
		Bridge < ::Nirvana::RuntimeProxy>::repository_id_,
		S::template __duplicate < ::Nirvana::RuntimeProxy>,
		S::template __release < ::Nirvana::RuntimeProxy>
	},
	{ // EPV
		S::_object
	}
};

template <class S>
class Servant <S, ::Nirvana::RuntimeProxy> : public ImplementationPseudo <S, ::Nirvana::RuntimeProxy>
{};

template <>
class ServantPOA < ::Nirvana::RuntimeProxy> : public ImplementationPseudoPOA < ::Nirvana::RuntimeProxy>
{
public:
	virtual Type < ::Nirvana::ConstPointer>::Var object () = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::RuntimeProxy> : public ImplementationPseudoStatic <S, ::Nirvana::RuntimeProxy>
{};

}
}
namespace POA_Nirvana {

typedef CORBA::Nirvana::ServantPOA <Nirvana::RuntimeProxy> RuntimeProxy;
template <class T> using RuntimeProxy_tie = CORBA::Nirvana::ServantTied <T, Nirvana::RuntimeProxy>;

}

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::RuntimeSupport>
{
public:
	static const typename Bridge < ::Nirvana::RuntimeSupport>::EPV epv_;

protected:
	static Interface* _runtime_proxy_get (Bridge < ::Nirvana::RuntimeSupport>* _b, Type < ::Nirvana::ConstPointer>::ABI_in obj, Interface* _env)
	{
		try {
			return TypeItf < ::Nirvana::RuntimeProxy>::ret (S::_implementation (_b).runtime_proxy_get (Type < ::Nirvana::ConstPointer>::in (obj)));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return TypeItf < ::Nirvana::RuntimeProxy>::ret ();
	}

	static void _runtime_proxy_remove (Bridge < ::Nirvana::RuntimeSupport>* _b, Type < ::Nirvana::ConstPointer>::ABI_in obj, Interface* _env)
	{
		try {
			S::_implementation (_b).runtime_proxy_remove (Type < ::Nirvana::ConstPointer>::in (obj));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

};

template <class S>
const Bridge < ::Nirvana::RuntimeSupport>::EPV Skeleton <S, ::Nirvana::RuntimeSupport>::epv_ = {
	{ // header
		Bridge < ::Nirvana::RuntimeSupport>::repository_id_,
		S::template __duplicate < ::Nirvana::RuntimeSupport>,
		S::template __release < ::Nirvana::RuntimeSupport>
	},
	{ // EPV
		S::_runtime_proxy_get,
		S::_runtime_proxy_remove
	}
};

template <class S>
class Servant <S, ::Nirvana::RuntimeSupport> : public ImplementationPseudo <S, ::Nirvana::RuntimeSupport>
{};

template <>
class ServantPOA < ::Nirvana::RuntimeSupport> : public ImplementationPseudoPOA < ::Nirvana::RuntimeSupport>
{
public:
	virtual TypeItf < ::Nirvana::RuntimeProxy>::Var runtime_proxy_get (Type < ::Nirvana::ConstPointer>::ConstRef obj) = 0;
	virtual void runtime_proxy_remove (Type < ::Nirvana::ConstPointer>::ConstRef obj) = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::RuntimeSupport> : public ImplementationPseudoStatic <S, ::Nirvana::RuntimeSupport>
{};

}
}
namespace POA_Nirvana {

typedef CORBA::Nirvana::ServantPOA <Nirvana::RuntimeSupport> RuntimeSupport;
template <class T> using RuntimeSupport_tie = CORBA::Nirvana::ServantTied <T, Nirvana::RuntimeSupport>;

}

#endif
