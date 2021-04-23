// This file was generated from "Runnable.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_RUNNABLE_S_H_
#define IDL_RUNNABLE_S_H_
#include "Runnable.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Legacy::Runnable>
{
public:
	static const typename Bridge < ::Nirvana::Legacy::Runnable>::EPV epv_;

protected:
	static void _run (Bridge < ::Nirvana::Legacy::Runnable>* _b, Interface* _env)
	{
		try {
			S::_implementation (_b).run ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

};

template <class S>
const Bridge < ::Nirvana::Legacy::Runnable>::EPV Skeleton <S, ::Nirvana::Legacy::Runnable>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Legacy::Runnable>::repository_id_,
		S::template __duplicate < ::Nirvana::Legacy::Runnable>,
		S::template __release < ::Nirvana::Legacy::Runnable>
	},
	{ // EPV
		S::_run
	}
};

template <class S>
class Servant <S, ::Nirvana::Legacy::Runnable> : public ImplementationPseudo <S, ::Nirvana::Legacy::Runnable>
{};

template <>
class ServantPOA < ::Nirvana::Legacy::Runnable> : public ImplementationPseudoPOA < ::Nirvana::Legacy::Runnable>
{
public:
	virtual void run () = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::Legacy::Runnable> : public ImplementationPseudoStatic <S, ::Nirvana::Legacy::Runnable>
{};

}
}
namespace POA_Nirvana {
namespace Legacy {

typedef CORBA::Nirvana::ServantPOA <Nirvana::Legacy::Runnable> Runnable;
template <class T> using Runnable_tie = CORBA::Nirvana::ServantTied <T, Nirvana::Legacy::Runnable>;

}
}

#endif
