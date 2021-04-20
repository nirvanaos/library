// This file was generated from "ModuleInit.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_MODULEINIT_S_H_
#define IDL_MODULEINIT_S_H_
#include "ModuleInit.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::ModuleInit>
{
public:
	static const typename Bridge < ::Nirvana::ModuleInit>::EPV epv_;

protected:
	static void _initialize (Bridge < ::Nirvana::ModuleInit>* _b, Interface* _env)
	{
		try {
			S::_implementation (_b).initialize ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
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
	}

};

template <class S>
const Bridge < ::Nirvana::ModuleInit>::EPV Skeleton <S, ::Nirvana::ModuleInit>::epv_ = {
	{ // header
		Bridge < ::Nirvana::ModuleInit>::repository_id_,
		S::template __duplicate < ::Nirvana::ModuleInit>,
		S::template __release < ::Nirvana::ModuleInit>
	},
	{ // EPV
		S::_initialize,
		S::_terminate
	}
};

template <class S>
class Servant <S, ::Nirvana::ModuleInit> : public ImplementationPseudo <S, ::Nirvana::ModuleInit>
{};

template <>
class ServantPOA < ::Nirvana::ModuleInit> : public ImplementationPseudoPOA < ::Nirvana::ModuleInit>
{
public:
	virtual void initialize () = 0;
	virtual void terminate () = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::ModuleInit> : public ImplementationPseudoStatic <S, ::Nirvana::ModuleInit>
{};

}
}
namespace POA_Nirvana {

typedef ::CORBA::Nirvana::ServantPOA <Nirvana::ModuleInit> ModuleInit;
template <class T> using ModuleInit_tie = ::CORBA::Nirvana::ServantTied <T, Nirvana::ModuleInit>;

}

#endif
