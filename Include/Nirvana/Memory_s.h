#ifndef NIRVANA_MEMORY_S_H_
#define NIRVANA_MEMORY_S_H_

#include "Memory.h"
#include <CORBA/ImplementationPseudo.h>
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Memory>
{
public:
	static const typename Bridge < ::Nirvana::Memory>::EPV epv_;

protected:
	static ::Nirvana::Pointer _allocate (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::UWord size, Flags flags, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).allocate (dst, size, flags);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _commit (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).commit (dst, size);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _decommit (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).decommit (dst, size);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _Memory_release (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).release (dst, size);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static ::Nirvana::Pointer _copy (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, ::Nirvana::UWord size, Flags flags, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).copy (dst, src, size, flags);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Boolean _is_readable (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).is_readable (p, size);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Boolean _is_writable (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).is_writable (p, size);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Boolean _is_private (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).is_private (p, size);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Boolean _is_copy (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p1, ::Nirvana::ConstPointer p2, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).is_copy (p1, p2, size);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ::Nirvana::Word _query (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::MemQuery param, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).query (p, param);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::Memory>::EPV Skeleton <S, ::Nirvana::Memory>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Memory>::interface_id_,
		S::template __duplicate < ::Nirvana::Memory>,
		S::template __release < ::Nirvana::Memory>
	},
	{ // epv
		S::_allocate,
		S::_commit,
		S::_decommit,
		S::_Memory_release,
		S::_copy,
		S::_is_readable,
		S::_is_writable,
		S::_is_private,
		S::_is_copy,
		S::_query
	}
};

/// Standard implementation of the Memory interface
template <class S>
class Servant <S, ::Nirvana::Memory> : public ImplementationPseudo <S, ::Nirvana::Memory>
{};

/// Static implementation of the Memory interface
template <class S>
class ServantStatic <S, ::Nirvana::Memory> : public ImplementationPseudoStatic <S, ::Nirvana::Memory>
{};

}
}

#endif
