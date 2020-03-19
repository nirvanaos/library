#ifndef NIRVANA_HEAPFACTORY_S_H_
#define NIRVANA_HEAPFACTORY_S_H_

#include "HeapFactory.h"
#include <CORBA/Server.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::HeapFactory>
{
public:
	static const typename Bridge < ::Nirvana::HeapFactory>::EPV epv_;

protected:
	static Interface* _create (Bridge < ::Nirvana::HeapFactory>* _b, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).create ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _create_with_granularity (Bridge < ::Nirvana::HeapFactory>* _b, ULong granularity, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).create_with_granularity (granularity);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::HeapFactory>::EPV Skeleton <S, ::Nirvana::HeapFactory>::epv_ = {
	{ // header
		Bridge < ::Nirvana::HeapFactory>::repository_id_,
		S::template __duplicate < ::Nirvana::HeapFactory>,
		S::template __release < ::Nirvana::HeapFactory>
	},
	{ // epv
		S::_create,
		S::_create_with_granularity
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Nirvana::HeapFactory> : public ImplementationPseudo <S, ::Nirvana::HeapFactory>
{};

// POA implementation
template <>
class ServantPOA < ::Nirvana::HeapFactory> : public ImplementationPseudo <ServantPOA < ::Nirvana::HeapFactory>, ::Nirvana::HeapFactory>
{
public:
	virtual I_ptr < ::Nirvana::Memory> create () = 0;
	virtual I_ptr < ::Nirvana::Memory> create_with_granularity () = 0;
};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::HeapFactory> : public ImplementationPseudoStatic <S, ::Nirvana::HeapFactory>
{};

}
}

#endif
