// This file was generated from "Process.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_PROCESS_S_H_
#define IDL_PROCESS_S_H_
#include "Process.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Legacy::Process>
{
public:
	static const typename Bridge < ::Nirvana::Legacy::Process>::EPV epv_;

protected:
	static Type <Short>::ABI_ret _main (Bridge < ::Nirvana::Legacy::Process>* _b, Type < ::Nirvana::Size>::ABI_in argc, Type < ::Nirvana::Pointer>::ABI_in argv, Interface* _env)
	{
		try {
			return Type <Short>::ret (S::_implementation (_b).main (Type < ::Nirvana::Size>::in (argc), Type < ::Nirvana::Pointer>::in (argv)));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Short>::ret ();
	}

};

template <class S>
const Bridge < ::Nirvana::Legacy::Process>::EPV Skeleton <S, ::Nirvana::Legacy::Process>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Legacy::Process>::repository_id_,
		S::template __duplicate < ::Nirvana::Legacy::Process>,
		S::template __release < ::Nirvana::Legacy::Process>
	},
	{ // EPV
		S::_main
	}
};

template <class S>
class Servant <S, ::Nirvana::Legacy::Process> : public ImplementationPseudo <S, ::Nirvana::Legacy::Process>
{};

template <>
class ServantPOA < ::Nirvana::Legacy::Process> : public ImplementationPseudoPOA < ::Nirvana::Legacy::Process>
{
public:
	virtual Type <Short>::Var main (Type < ::Nirvana::Size>::ConstRef argc, Type < ::Nirvana::Pointer>::ConstRef argv) = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::Legacy::Process> : public ImplementationPseudoStatic <S, ::Nirvana::Legacy::Process>
{};

}
}
namespace POA_Nirvana {
namespace Legacy {

typedef CORBA::Nirvana::ServantPOA <Nirvana::Legacy::Process> Process;
template <class T> using Process_tie = CORBA::Nirvana::ServantTied <T, Nirvana::Legacy::Process>;

}
}

#endif
