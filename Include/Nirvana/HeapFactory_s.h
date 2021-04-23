// This file was generated from "HeapFactory.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_HEAPFACTORY_S_H_
#define IDL_HEAPFACTORY_S_H_
#include "HeapFactory.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::HeapFactory>
{
public:
	static const typename Bridge < ::Nirvana::HeapFactory>::EPV epv_;

protected:
	static Interface* _create (Bridge < ::Nirvana::HeapFactory>* _b, Interface* _env)
	{
		try {
			return TypeItf < ::Nirvana::Memory>::ret (S::_implementation (_b).create ());
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return TypeItf < ::Nirvana::Memory>::ret ();
	}

	static Interface* _create_with_granularity (Bridge < ::Nirvana::HeapFactory>* _b, Type <ULong>::ABI_in granularity, Interface* _env)
	{
		try {
			return TypeItf < ::Nirvana::Memory>::ret (S::_implementation (_b).create_with_granularity (Type <ULong>::in (granularity)));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return TypeItf < ::Nirvana::Memory>::ret ();
	}

};

template <class S>
const Bridge < ::Nirvana::HeapFactory>::EPV Skeleton <S, ::Nirvana::HeapFactory>::epv_ = {
	{ // header
		Bridge < ::Nirvana::HeapFactory>::repository_id_,
		S::template __duplicate < ::Nirvana::HeapFactory>,
		S::template __release < ::Nirvana::HeapFactory>
	},
	{ // EPV
		S::_create,
		S::_create_with_granularity
	}
};

template <class S>
class Servant <S, ::Nirvana::HeapFactory> : public ImplementationPseudo <S, ::Nirvana::HeapFactory>
{};

template <>
class ServantPOA < ::Nirvana::HeapFactory> : public ImplementationPseudoPOA < ::Nirvana::HeapFactory>
{
public:
	virtual TypeItf < ::Nirvana::Memory>::Var create () = 0;
	virtual TypeItf < ::Nirvana::Memory>::Var create_with_granularity (Type <ULong>::ConstRef granularity) = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::HeapFactory> : public ImplementationPseudoStatic <S, ::Nirvana::HeapFactory>
{};

}
}
namespace POA_Nirvana {

typedef CORBA::Nirvana::ServantPOA <Nirvana::HeapFactory> HeapFactory;
template <class T> using HeapFactory_tie = CORBA::Nirvana::ServantTied <T, Nirvana::HeapFactory>;

}

#endif
