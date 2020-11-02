#ifndef NIRVANA_MODULEINIT_S_H_
#define NIRVANA_MODULEINIT_S_H_

#include "ModuleInit.h"
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::ModuleInit>
{
public:
	static const typename Bridge < ::Nirvana::ModuleInit>::EPV epv_;

protected:
	static ABI_boolean _initialize (Bridge < ::Nirvana::ModuleInit>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).initialize ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _terminate (Bridge < ::Nirvana::ModuleInit>* _b, Interface* _env)
	{
		try {
			S::_implementation (_b).terminate ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::ModuleInit>::EPV Skeleton <S, ::Nirvana::ModuleInit>::epv_ = {
	{ // header
		Bridge < ::Nirvana::ModuleInit>::repository_id_,
		S::template __duplicate < ::Nirvana::ModuleInit>,
		S::template __release < ::Nirvana::ModuleInit>
	},
	{ // epv
		S::_initialize,
		S::_terminate
	}
};

/// Static implementation
template <class S>
class ServantStatic <S, ::Nirvana::Module> : public ImplementationPseudoStatic <S, ::Nirvana::Module>
{};

}
}

#endif
