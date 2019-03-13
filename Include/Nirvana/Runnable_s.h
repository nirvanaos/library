#ifndef NIRVANA_RUNNABLE_S_H_
#define NIRVANA_RUNNABLE_S_H_

#include "Runnable_c.h"
#include <Servant.h>

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
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
	}
};

template <class S>
const Bridge < ::Nirvana::Runnable>::EPV Skeleton <S, ::Nirvana::Runnable>::epv_ = {
	{ // interface
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
	public ImplementationSingle <S, ::Nirvana::Runnable>
{};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::Runnable> :
	public ImplementationStaticSingle <S, ::Nirvana::Runnable>
{};

// POA (virtual) implementation

template <>
class ServantPOA <::Nirvana::Runnable> :
	public ImplementationSingle <ServantPOA <::Nirvana::Runnable>, ::Nirvana::Runnable>
{
public:
	virtual void run () = 0;
};

}
}

#endif
