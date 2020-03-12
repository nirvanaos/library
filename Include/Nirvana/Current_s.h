#ifndef NIRVANA_CURRENT_S_H_
#define NIRVANA_CURRENT_S_H_

#include "Current.h"
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Current>
{
public:
	static const typename Bridge < ::Nirvana::Current>::EPV epv_;

protected:
	static ::Nirvana::DeadlineTime _get_deadline (Bridge < ::Nirvana::Current>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).deadline ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::Current>::EPV Skeleton <S, ::Nirvana::Current>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Current>::interface_id_,
		S::template __duplicate < ::Nirvana::Current>,
		S::template __release < ::Nirvana::Current>
	},
	{ // epv
		S::_get_deadline
	}
};

/// Static implementation
template <class S>
class ServantStatic <S, ::Nirvana::Current> : public ImplementationPseudoStatic <S, ::Nirvana::Current>
{};

}
}

#endif
