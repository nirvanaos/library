// This file was generated from "Memory.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_MEMORY_S_H_
#define IDL_MEMORY_S_H_
#include "Memory.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Memory>
{
public:
	static const typename Bridge < ::Nirvana::Memory>::EPV epv_;

protected:
	static Type < ::Nirvana::Pointer>::ABI_ret _allocate (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in dst, Type < ::Nirvana::Size>::ABI_in size, Type <UShort>::ABI_in flags, Interface* _env)
	{
		try {
			return Type < ::Nirvana::Pointer>::ret (S::_implementation (_b).allocate (Type < ::Nirvana::Pointer>::in (dst), Type < ::Nirvana::Size>::in (size), Type <UShort>::in (flags)));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type < ::Nirvana::Pointer>::ret ();
	}

	static void _commit (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in dst, Type < ::Nirvana::Size>::ABI_in size, Interface* _env)
	{
		try {
			S::_implementation (_b).commit (Type < ::Nirvana::Pointer>::in (dst), Type < ::Nirvana::Size>::in (size));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _decommit (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in dst, Type < ::Nirvana::Size>::ABI_in size, Interface* _env)
	{
		try {
			S::_implementation (_b).decommit (Type < ::Nirvana::Pointer>::in (dst), Type < ::Nirvana::Size>::in (size));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _release (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in dst, Type < ::Nirvana::Size>::ABI_in size, Interface* _env)
	{
		try {
			S::_implementation (_b).release (Type < ::Nirvana::Pointer>::in (dst), Type < ::Nirvana::Size>::in (size));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static Type < ::Nirvana::Pointer>::ABI_ret _copy (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in dst, Type < ::Nirvana::Pointer>::ABI_in src, Type < ::Nirvana::Size>::ABI_in size, Type <UShort>::ABI_in flags, Interface* _env)
	{
		try {
			return Type < ::Nirvana::Pointer>::ret (S::_implementation (_b).copy (Type < ::Nirvana::Pointer>::in (dst), Type < ::Nirvana::Pointer>::in (src), Type < ::Nirvana::Size>::in (size), Type <UShort>::in (flags)));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type < ::Nirvana::Pointer>::ret ();
	}

	static Type <Boolean>::ABI_ret _is_readable (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in p, Type < ::Nirvana::Size>::ABI_in size, Interface* _env)
	{
		try {
			return Type <Boolean>::ret (S::_implementation (_b).is_readable (Type < ::Nirvana::ConstPointer>::in (p), Type < ::Nirvana::Size>::in (size)));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Boolean>::ret ();
	}

	static Type <Boolean>::ABI_ret _is_writable (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in p, Type < ::Nirvana::Size>::ABI_in size, Interface* _env)
	{
		try {
			return Type <Boolean>::ret (S::_implementation (_b).is_writable (Type < ::Nirvana::ConstPointer>::in (p), Type < ::Nirvana::Size>::in (size)));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Boolean>::ret ();
	}

	static Type <Boolean>::ABI_ret _is_private (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in p, Type < ::Nirvana::Size>::ABI_in size, Interface* _env)
	{
		try {
			return Type <Boolean>::ret (S::_implementation (_b).is_private (Type < ::Nirvana::ConstPointer>::in (p), Type < ::Nirvana::Size>::in (size)));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Boolean>::ret ();
	}

	static Type <Boolean>::ABI_ret _is_copy (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in p1, Type < ::Nirvana::ConstPointer>::ABI_in p2, Type < ::Nirvana::Size>::ABI_in size, Interface* _env)
	{
		try {
			return Type <Boolean>::ret (S::_implementation (_b).is_copy (Type < ::Nirvana::ConstPointer>::in (p1), Type < ::Nirvana::ConstPointer>::in (p2), Type < ::Nirvana::Size>::in (size)));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Boolean>::ret ();
	}

	static Type < ::Nirvana::UIntPtr>::ABI_ret _query (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in p, ABI_enum param, Interface* _env)
	{
		try {
			return Type < ::Nirvana::UIntPtr>::ret (S::_implementation (_b).query (Type < ::Nirvana::ConstPointer>::in (p), Type <Definitions < ::Nirvana::Memory>::QueryParam>::in (param)));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type < ::Nirvana::UIntPtr>::ret ();
	}

};

template <class S>
const Bridge < ::Nirvana::Memory>::EPV Skeleton <S, ::Nirvana::Memory>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Memory>::repository_id_,
		S::template __duplicate < ::Nirvana::Memory>,
		S::template __release < ::Nirvana::Memory>
	},
	{ // EPV
		S::_allocate,
		S::_commit,
		S::_decommit,
		S::_release,
		S::_copy,
		S::_is_readable,
		S::_is_writable,
		S::_is_private,
		S::_is_copy,
		S::_query
	}
};

template <class S>
class Servant <S, ::Nirvana::Memory> : public ImplementationPseudo <S, ::Nirvana::Memory>
{};

template <>
class ServantPOA < ::Nirvana::Memory> : public ImplementationPseudoPOA < ::Nirvana::Memory>
{
public:
	virtual Type < ::Nirvana::Pointer>::Var allocate (Type < ::Nirvana::Pointer>::ConstRef dst, Type < ::Nirvana::Size>::ConstRef size, Type <UShort>::ConstRef flags) = 0;
	virtual void commit (Type < ::Nirvana::Pointer>::ConstRef dst, Type < ::Nirvana::Size>::ConstRef size) = 0;
	virtual void decommit (Type < ::Nirvana::Pointer>::ConstRef dst, Type < ::Nirvana::Size>::ConstRef size) = 0;
	virtual void release (Type < ::Nirvana::Pointer>::ConstRef dst, Type < ::Nirvana::Size>::ConstRef size) = 0;
	virtual Type < ::Nirvana::Pointer>::Var copy (Type < ::Nirvana::Pointer>::ConstRef dst, Type < ::Nirvana::Pointer>::ConstRef src, Type < ::Nirvana::Size>::ConstRef size, Type <UShort>::ConstRef flags) = 0;
	virtual Type <Boolean>::Var is_readable (Type < ::Nirvana::ConstPointer>::ConstRef p, Type < ::Nirvana::Size>::ConstRef size) = 0;
	virtual Type <Boolean>::Var is_writable (Type < ::Nirvana::ConstPointer>::ConstRef p, Type < ::Nirvana::Size>::ConstRef size) = 0;
	virtual Type <Boolean>::Var is_private (Type < ::Nirvana::ConstPointer>::ConstRef p, Type < ::Nirvana::Size>::ConstRef size) = 0;
	virtual Type <Boolean>::Var is_copy (Type < ::Nirvana::ConstPointer>::ConstRef p1, Type < ::Nirvana::ConstPointer>::ConstRef p2, Type < ::Nirvana::Size>::ConstRef size) = 0;
	virtual Type < ::Nirvana::UIntPtr>::Var query (Type < ::Nirvana::ConstPointer>::ConstRef p, Type <Definitions < ::Nirvana::Memory>::QueryParam>::ConstRef param) = 0;
};

template <class S>
class ServantStatic <S, ::Nirvana::Memory> : public ImplementationPseudoStatic <S, ::Nirvana::Memory>
{};

}
}
namespace POA_Nirvana {

typedef CORBA::Nirvana::ServantPOA <Nirvana::Memory> Memory;
template <class T> using Memory_tie = CORBA::Nirvana::ServantTied <T, Nirvana::Memory>;

}

#endif
