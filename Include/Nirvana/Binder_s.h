#ifndef NIRVANA_BINDER_S_H_
#define NIRVANA_BINDER_S_H_

#include "Binder.h"
#include "CORBA/ImplementationPseudo.h"
#include "CORBA/ImplementationPseudoStatic.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Binder>
{
public:
	static const typename Bridge <::Nirvana::Binder>::EPV epv_;

protected:
	static Interface* _bind (Bridge <::Nirvana::Binder>* obj, Type <String>::ABI_in name, Type <String>::ABI_in interface_id, EnvironmentBridge* env)
	{
		try {
			return TypeI <Interface>::ret (S::_implementation (obj).bind (Type <String>::in (name), Type <String>::in (interface_id)));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}
};

template <class S>
const Bridge <::Nirvana::Binder>::EPV Skeleton <S, ::Nirvana::Binder>::epv_ = {
	{ // header
		Bridge <::Nirvana::Binder>::repository_id_,
		S::template __duplicate <::Nirvana::Binder>,
		S::template __release <::Nirvana::Binder>
	},
	{ // epv
		S::_bind
	}
};

template <class S>
class Servant <S, ::Nirvana::Binder> :
	public ImplementationPseudo <S, ::Nirvana::Binder>
{};

template <class S>
class ServantStatic <S, ::Nirvana::Binder> :
	public ImplementationPseudoStatic <S, ::Nirvana::Binder>
{};

}
}

#endif
