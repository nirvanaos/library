#ifndef NIRVANA_ORB_BINDER_S_H_
#define NIRVANA_ORB_BINDER_S_H_

#include "Binder_c.h"
#include "CORBA/ImplementationStatic.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, Binder>
{
public:
	static const typename Bridge <Binder>::EPV epv_;

protected:
	static Bridge <Interface>* _bind (Bridge <Binder>* obj, const StringABI <char>* name, const StringABI <char>* interface_id, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).bind (std::string::_unmarshal (name), std::string::_unmarshal (interface_id));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}
};

template <class S>
const Bridge <Binder>::EPV Skeleton <S, Binder>::epv_ = {
	{ // interface
		Bridge <Binder>::interface_id_,
		S::template __duplicate <Binder>,
		S::template __release <Binder>
	},
	{ // epv
		S::_bind
	}
};

template <class S>
class ServantStatic <S, Binder> :
	public ImplementationStaticPseudo <S, Binder>
{};

}
}

#endif
