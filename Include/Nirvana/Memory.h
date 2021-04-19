// This file was generated from "Memory.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_MEMORY_H_
#define IDL_MEMORY_H_

namespace Nirvana {

class Memory;
typedef ::CORBA::Nirvana::TypeItf <Memory>::C_ptr Memory_ptr;
typedef ::CORBA::Nirvana::TypeItf <Memory>::C_var Memory_var;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Definitions < ::Nirvana::Memory>
{
	static const UShort READ_WRITE = 0;
	static const UShort READ_ONLY = 1;
	static const UShort RESERVED = 2;
	static const UShort DST_ALLOCATE = 8;
	static const UShort SRC_DECOMMIT = 16;
	static const UShort SRC_RELEASE = 48;
	static const UShort ZERO_INIT = 64;
	static const UShort EXACTLY = 128;
	static const UShort HARDWARE_PROTECTION = 1;
	static const UShort COPY_ON_WRITE = 2;
	static const UShort SPACE_RESERVATION = 4;
	static const UShort ACCESS_CHECK = 8;

	enum class QueryParam : ::CORBA::Nirvana::ABI_enum
	{
		ALLOCATION_UNIT,
		PROTECTION_UNIT,
		COMMIT_UNIT,
		OPTIMAL_COMMIT_UNIT,
		SHARING_UNIT,
		SHARING_ASSOCIATIVITY,
		GRANULARITY,
		ALLOCATION_SPACE_BEGIN,
		ALLOCATION_SPACE_END,
		FLAGS
	};
};

template <>
const Char RepIdOf <Definitions < ::Nirvana::Memory>::QueryParam>::repository_id_ [] = "IDL:Nirvana/Memory/QueryParam:1.0";

template <>
struct Type <Definitions < ::Nirvana::Memory>::QueryParam> : public TypeEnum <Definitions < ::Nirvana::Memory>::QueryParam, Definitions < ::Nirvana::Memory>::QueryParam::FLAGS>
{};

NIRVANA_BRIDGE_BEGIN (::Nirvana::Memory, "IDL:Nirvana/Memory:1.0")
Type < ::Nirvana::Pointer>::ABI_ret (*allocate) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type <UShort>::ABI_in, Interface* _env);
void (*commit) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface* _env);
void (*decommit) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface* _env);
void (*release) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface* _env);
Type < ::Nirvana::Pointer>::ABI_ret (*copy) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::Pointer>::ABI_in, Type < ::Nirvana::Pointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Type <UShort>::ABI_in, Interface* _env);
Type <Boolean>::ABI_ret (*is_readable) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface* _env);
Type <Boolean>::ABI_ret (*is_writable) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface* _env);
Type <Boolean>::ABI_ret (*is_private) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface* _env);
Type <Boolean>::ABI_ret (*is_copy) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in, Type < ::Nirvana::ConstPointer>::ABI_in, Type < ::Nirvana::Size>::ABI_in, Interface* _env);
Type < ::Nirvana::UIntPtr>::ABI_ret (*query) (Bridge < ::Nirvana::Memory>* _b, Type < ::Nirvana::ConstPointer>::ABI_in, Type <Definitions < ::Nirvana::Memory>::QueryParam>::ABI_in, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Memory> :
	public T,
	public Definitions < ::Nirvana::Memory>
{
public:
	Type < ::Nirvana::Pointer>::Var allocate (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Size>::C_in size, Type <UShort>::C_in flags);
	void commit (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Size>::C_in size);
	void decommit (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Size>::C_in size);
	void release (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Size>::C_in size);
	Type < ::Nirvana::Pointer>::Var copy (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Pointer>::C_in src, Type < ::Nirvana::Size>::C_in size, Type <UShort>::C_in flags);
	Type <Boolean>::Var is_readable (Type < ::Nirvana::ConstPointer>::C_in p, Type < ::Nirvana::Size>::C_in size);
	Type <Boolean>::Var is_writable (Type < ::Nirvana::ConstPointer>::C_in p, Type < ::Nirvana::Size>::C_in size);
	Type <Boolean>::Var is_private (Type < ::Nirvana::ConstPointer>::C_in p, Type < ::Nirvana::Size>::C_in size);
	Type <Boolean>::Var is_copy (Type < ::Nirvana::ConstPointer>::C_in p1, Type < ::Nirvana::ConstPointer>::C_in p2, Type < ::Nirvana::Size>::C_in size);
	Type < ::Nirvana::UIntPtr>::Var query (Type < ::Nirvana::ConstPointer>::C_in p, Type <Definitions < ::Nirvana::Memory>::QueryParam>::C_in param);
};

template <class T>
Type < ::Nirvana::Pointer>::Var Client <T, ::Nirvana::Memory>::allocate (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Size>::C_in size, Type <UShort>::C_in flags)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::Pointer>::C_ret _ret = (_b._epv ().epv.allocate) (&_b, &dst, &size, &flags, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Nirvana::Memory>::commit (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Size>::C_in size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.commit) (&_b, &dst, &size, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::Memory>::decommit (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Size>::C_in size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.decommit) (&_b, &dst, &size, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::Memory>::release (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Size>::C_in size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.release) (&_b, &dst, &size, &_env);
	_env.check ();
}

template <class T>
Type < ::Nirvana::Pointer>::Var Client <T, ::Nirvana::Memory>::copy (Type < ::Nirvana::Pointer>::C_in dst, Type < ::Nirvana::Pointer>::C_in src, Type < ::Nirvana::Size>::C_in size, Type <UShort>::C_in flags)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::Pointer>::C_ret _ret = (_b._epv ().epv.copy) (&_b, &dst, &src, &size, &flags, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Type <Boolean>::Var Client <T, ::Nirvana::Memory>::is_readable (Type < ::Nirvana::ConstPointer>::C_in p, Type < ::Nirvana::Size>::C_in size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_readable) (&_b, &p, &size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Type <Boolean>::Var Client <T, ::Nirvana::Memory>::is_writable (Type < ::Nirvana::ConstPointer>::C_in p, Type < ::Nirvana::Size>::C_in size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_writable) (&_b, &p, &size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Type <Boolean>::Var Client <T, ::Nirvana::Memory>::is_private (Type < ::Nirvana::ConstPointer>::C_in p, Type < ::Nirvana::Size>::C_in size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_private) (&_b, &p, &size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Type <Boolean>::Var Client <T, ::Nirvana::Memory>::is_copy (Type < ::Nirvana::ConstPointer>::C_in p1, Type < ::Nirvana::ConstPointer>::C_in p2, Type < ::Nirvana::Size>::C_in size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_copy) (&_b, &p1, &p2, &size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Type < ::Nirvana::UIntPtr>::Var Client <T, ::Nirvana::Memory>::query (Type < ::Nirvana::ConstPointer>::C_in p, Type <Definitions < ::Nirvana::Memory>::QueryParam>::C_in param)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::UIntPtr>::C_ret _ret = (_b._epv ().epv.query) (&_b, &p, &param, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Memory : public CORBA::Nirvana::ClientInterface <Memory>
{
public:
	using ::CORBA::Nirvana::Definitions <Memory>::READ_WRITE;
	using ::CORBA::Nirvana::Definitions <Memory>::READ_ONLY;
	using ::CORBA::Nirvana::Definitions <Memory>::RESERVED;
	using ::CORBA::Nirvana::Definitions <Memory>::DST_ALLOCATE;
	using ::CORBA::Nirvana::Definitions <Memory>::SRC_DECOMMIT;
	using ::CORBA::Nirvana::Definitions <Memory>::SRC_RELEASE;
	using ::CORBA::Nirvana::Definitions <Memory>::ZERO_INIT;
	using ::CORBA::Nirvana::Definitions <Memory>::EXACTLY;
	using ::CORBA::Nirvana::Definitions <Memory>::HARDWARE_PROTECTION;
	using ::CORBA::Nirvana::Definitions <Memory>::COPY_ON_WRITE;
	using ::CORBA::Nirvana::Definitions <Memory>::SPACE_RESERVATION;
	using ::CORBA::Nirvana::Definitions <Memory>::ACCESS_CHECK;
	using ::CORBA::Nirvana::Definitions <Memory>::QueryParam;
};

}

#endif
