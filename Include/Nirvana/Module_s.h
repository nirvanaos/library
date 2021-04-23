// This file was generated from "Module.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_MODULE_S_H_
#define IDL_MODULE_S_H_
#include "Module.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Module>
{
public:
	static const typename Bridge < ::Nirvana::Module>::EPV epv_;

protected:
	static Type < ::Nirvana::ConstPointer>::ABI_ret __get_base_address (Bridge < ::Nirvana::Module>* _b, Interface* _env)
	{
		try {
			return Type < ::Nirvana::ConstPointer>::ret (S::_implementation (_b).base_address ());
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type < ::Nirvana::ConstPointer>::ret ();
	}

};

template <class S>
const Bridge < ::Nirvana::Module>::EPV Skeleton <S, ::Nirvana::Module>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Module>::repository_id_,
		S::template __duplicate < ::Nirvana::Module>,
		S::template __release < ::Nirvana::Module>
	},
	{ // EPV
		S::__get_base_address
	}
};

template <class S>
class Servant <S, ::Nirvana::Module> : public ImplementationPseudo <S, ::Nirvana::Module>
{};

template <>
class ServantPOA < ::Nirvana::Module> : public ImplementationPseudoPOA < ::Nirvana::Module>
{
public:
	virtual Type < ::Nirvana::ConstPointer>::Var base_address () = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::Module> : public ImplementationPseudoStatic <S, ::Nirvana::Module>
{};

}
}
namespace POA_Nirvana {

typedef CORBA::Nirvana::ServantPOA <Nirvana::Module> Module;
template <class T> using Module_tie = CORBA::Nirvana::ServantTied <T, Nirvana::Module>;

}

#endif
