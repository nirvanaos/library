#ifndef NIRVANA_MODULE_S_H_
#define NIRVANA_MODULE_S_H_

#include "Module.h"
#include <CORBA/ImplementationPseudo.h>
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Module>
{
public:
	static const typename Bridge < ::Nirvana::Module>::EPV epv_;

protected:
	static ::Nirvana::ConstPointer _get_base_address (Bridge < ::Nirvana::Module>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).base_address ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::Module>::EPV Skeleton <S, ::Nirvana::Module>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Module>::repository_id_,
		S::template __duplicate < ::Nirvana::Module>,
		S::template __release < ::Nirvana::Module>
	},
	{ // epv
		S::_get_base_address
	}
};

/// Standard implementation
template <class S>
class Servant <S, ::Nirvana::Module> : public ImplementationPseudo <S, ::Nirvana::Module>
{};

/// Static implementation
template <class S>
class ServantStatic <S, ::Nirvana::Module> : public ImplementationPseudoStatic <S, ::Nirvana::Module>
{};

}
}

#endif
