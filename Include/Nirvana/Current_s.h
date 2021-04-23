// This file was generated from "Current.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_CURRENT_S_H_
#define IDL_CURRENT_S_H_
#include "Current.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Current>
{
public:
	static const typename Bridge < ::Nirvana::Current>::EPV epv_;

protected:
	static Type <ULongLong>::ABI_ret __get_deadline (Bridge < ::Nirvana::Current>* _b, Interface* _env)
	{
		try {
			return Type <ULongLong>::ret (S::_implementation (_b).deadline ());
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <ULongLong>::ret ();
	}

};

template <class S>
const Bridge < ::Nirvana::Current>::EPV Skeleton <S, ::Nirvana::Current>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Current>::repository_id_,
		S::template __duplicate < ::Nirvana::Current>,
		S::template __release < ::Nirvana::Current>
	},
	{ // EPV
		S::__get_deadline
	}
};

template <class S>
class Servant <S, ::Nirvana::Current> : public ImplementationPseudo <S, ::Nirvana::Current>
{};

template <>
class ServantPOA < ::Nirvana::Current> : public ImplementationPseudoPOA < ::Nirvana::Current>
{
public:
	virtual Type <ULongLong>::Var deadline () = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::Current> : public ImplementationPseudoStatic <S, ::Nirvana::Current>
{};

}
}
namespace POA_Nirvana {

typedef CORBA::Nirvana::ServantPOA <Nirvana::Current> Current;
template <class T> using Current_tie = CORBA::Nirvana::ServantTied <T, Nirvana::Current>;

}

#endif
