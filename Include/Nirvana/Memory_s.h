#ifndef NIRVANA_MEMORY_S_H_
#define NIRVANA_MEMORY_S_H_

#include "Memory_c.h"
#include <Servant.h>

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
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static void _commit (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).commit (dst, size);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
	}

	static void _decommit (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).decommit (dst, size);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
	}

	static void _Memory_release (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).release (dst, size);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
	}

	static ::Nirvana::Pointer _copy (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, ::Nirvana::UWord size, Flags flags, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).copy (dst, src, size, flags);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean _is_readable (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).is_readable (p, size);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean _is_writable (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).is_writable (p, size);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean _is_private (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).is_private (p, size);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean _is_copy (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p1, ::Nirvana::ConstPointer p2, ::Nirvana::UWord size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).is_copy (p1, p2, size);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static ::Nirvana::Word _query (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, Bridge < ::Nirvana::Memory>::QueryParam param, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).query (p, param);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::Memory>::EPV Skeleton <S, ::Nirvana::Memory>::epv_ = {
	{ // interface
		Bridge < ::Nirvana::Memory>::interface_id_,
		S::template __duplicate < ::Nirvana::Memory>,
		S::template __release < ::Nirvana::Memory>
	},
	{ // base
		S::template _wide <AbstractBase, ::Nirvana::Memory>
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

// Standard implementation

template <class S>
class Servant <S, ::Nirvana::Memory> : public Implementation <S, ::Nirvana::Memory, ::CORBA::AbstractBase>
{};

// POA implementation
template <>
class ServantPOA < ::Nirvana::Memory> : public ImplementationPOA < ::Nirvana::Memory, ::CORBA::AbstractBase>
{
public:
	virtual ::Nirvana::Pointer allocate (::Nirvana::Pointer dst, ::Nirvana::UWord size, Flags flags) = 0;
	virtual void commit (::Nirvana::Pointer dst, ::Nirvana::UWord size) = 0;
	virtual void decommit (::Nirvana::Pointer dst, ::Nirvana::UWord size) = 0;
	virtual void release (::Nirvana::Pointer dst, ::Nirvana::UWord size) = 0;
	virtual ::Nirvana::Pointer copy (::Nirvana::Pointer dst, ::Nirvana::Pointer src, ::Nirvana::UWord size, Flags flags) = 0;
	virtual Boolean is_readable (::Nirvana::ConstPointer p, ::Nirvana::UWord size) = 0;
	virtual Boolean is_writable (::Nirvana::ConstPointer p, ::Nirvana::UWord size) = 0;
	virtual Boolean is_private (::Nirvana::ConstPointer p, ::Nirvana::UWord size) = 0;
	virtual Boolean is_copy (::Nirvana::ConstPointer p1, ::Nirvana::ConstPointer p2, ::Nirvana::UWord size) = 0;
	virtual ::Nirvana::Word query (::Nirvana::ConstPointer p, Bridge < ::Nirvana::Memory>::QueryParam param) = 0;
};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::Memory> : public ImplementationStatic <S, ::Nirvana::Memory, ::CORBA::AbstractBase>
{};

}
}

#endif
