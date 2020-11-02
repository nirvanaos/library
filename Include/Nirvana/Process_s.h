#ifndef NIRVANA_LEGACY_PROCESS_S_H_
#define NIRVANA_LEGACY_PROCESS_S_H_

#include "Process.h"
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Legacy::Process>
{
public:
	static const typename Bridge < ::Nirvana::Legacy::Process>::EPV epv_;

protected:
	static ::Nirvana::Word _run (Bridge < ::Nirvana::Legacy::Process>* _b, ::Nirvana::Size argc, Char** argv, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).run (argc, argv);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

template <class S>
const Bridge < ::Nirvana::Legacy::Process>::EPV Skeleton <S, ::Nirvana::Legacy::Process>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Legacy::Process>::repository_id_,
		S::template __duplicate < ::Nirvana::Legacy::Process>,
		S::template __release < ::Nirvana::Legacy::Process>
	},
	{ // epv
		S::_run
	}
};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::Legacy::Process> :
	public ImplementationPseudoStatic <S, ::Nirvana::Legacy::Process>
{};

}
}

#endif
