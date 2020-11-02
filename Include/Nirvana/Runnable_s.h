#ifndef NIRVANA_LEGACY_RUNNABLE_S_H_
#define NIRVANA_LEGACY_RUNNABLE_S_H_

#include "Runnable.h"
#include <CORBA/ImplementationPseudo.h>
#include <CORBA/ImplementationPseudoStatic.h>
#include <CORBA/ImplementationPseudoPOA.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Legacy::Runnable>
{
public:
	static const typename Bridge < ::Nirvana::Legacy::Runnable>::EPV epv_;

protected:
	static void _run (Bridge < ::Nirvana::Legacy::Runnable>* _b, EnvironmentBridge* _env)
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
	{ // epv
		S::_run
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Nirvana::Legacy::Runnable> :
	public ImplementationPseudo <S, ::Nirvana::Legacy::Runnable>
{};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::Legacy::Runnable> :
	public ImplementationPseudoStatic <S, ::Nirvana::Legacy::Runnable>
{};

// POA - style (virtual) implementation

template <>
class ServantPOA < ::Nirvana::Legacy::Runnable> : 
	public ImplementationPseudoPOA < ::Nirvana::Legacy::Runnable>
{
public:
	virtual void run () = 0;
};

}
}

#endif
