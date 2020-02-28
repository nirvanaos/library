#ifndef NIRVANA_RUNNABLE_S_H_
#define NIRVANA_RUNNABLE_S_H_

#include "Runnable.h"
#include <CORBA/ImplementationPseudo.h>
#include <CORBA/ImplementationPseudoStatic.h>
#include <CORBA/ImplementationPseudoPOA.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Runnable>
{
public:
	static const typename Bridge < ::Nirvana::Runnable>::EPV epv_;

protected:
	static void _run (Bridge < ::Nirvana::Runnable>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).run ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

template <class S>
const Bridge < ::Nirvana::Runnable>::EPV Skeleton <S, ::Nirvana::Runnable>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Runnable>::interface_id_,
		S::template __duplicate < ::Nirvana::Runnable>,
		S::template __release < ::Nirvana::Runnable>
	},
	{ // epv
		S::_run
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Nirvana::Runnable> :
	public ImplementationPseudo <S, ::Nirvana::Runnable>
{};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::Runnable> :
	public ImplementationPseudoStatic <S, ::Nirvana::Runnable>
{};

// POA - style (virtual) implementation

template <>
class ServantPOA < ::Nirvana::Runnable> : 
	public ImplementationPseudoPOA < ::Nirvana::Runnable>
{
public:
	virtual void run () = 0;
};

}
}

#endif
