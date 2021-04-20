// This file was generated from "Binder.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_BINDER_S_H_
#define IDL_BINDER_S_H_
#include "Binder.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Binder>
{
public:
	static const typename Bridge < ::Nirvana::Binder>::EPV epv_;

protected:
	static Interface* _bind (Bridge < ::Nirvana::Binder>* _b, Type <String>::ABI_in name, Type <String>::ABI_in interface_id, Interface* _env)
	{
		try {
			return TypeItf <Interface>::ret (S::_implementation (_b).bind (Type <String>::in (name), Type <String>::in (interface_id)));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return TypeItf <Interface>::ret ();
	}

};

template <class S>
const Bridge < ::Nirvana::Binder>::EPV Skeleton <S, ::Nirvana::Binder>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Binder>::repository_id_,
		S::template __duplicate < ::Nirvana::Binder>,
		S::template __release < ::Nirvana::Binder>
	},
	{ // EPV
		S::_bind
	}
};

template <class S>
class Servant <S, ::Nirvana::Binder> : public ImplementationPseudo <S, ::Nirvana::Binder>
{};

template <>
class ServantPOA < ::Nirvana::Binder> : public ImplementationPseudoPOA < ::Nirvana::Binder>
{
public:
	virtual TypeItf <Interface>::Var bind (Type <String>::ConstRef name, Type <String>::ConstRef interface_id) = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::Binder> : public ImplementationPseudoStatic <S, ::Nirvana::Binder>
{};

}
}
namespace POA_Nirvana {

typedef ::CORBA::Nirvana::ServantPOA <Nirvana::Binder> Binder;
template <class T> using Binder_tie = ::CORBA::Nirvana::ServantTied <T, Nirvana::Binder>;

}

#endif
