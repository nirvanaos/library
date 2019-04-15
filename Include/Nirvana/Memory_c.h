#ifndef NIRVANA_MEMORY_C_H_
#define NIRVANA_MEMORY_C_H_

#include <CORBA/Interface_c.h>
#include "Nirvana.h"

namespace Nirvana {

class Memory;
typedef ::CORBA::Nirvana::T_ptr <Memory> Memory_ptr;
typedef ::CORBA::Nirvana::T_var <Memory> Memory_var;
typedef ::CORBA::Nirvana::T_out <Memory> Memory_out;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge < ::Nirvana::Memory> :
	public BridgeMarshal < ::Nirvana::Memory>
{
public:
	enum
	{
		READ_WRITE = 0x00,
		READ_ONLY = 0x01,
		RESERVED = 0x02,

		ALLOCATE = 0x08,
		DECOMMIT = 0x10,
		RELEASE = 0x30,

		ZERO_INIT = 0x40,
		EXACTLY = 0x80
	};

	enum QueryParam
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

	// Flags
	enum
	{
		HARDWARE_PROTECTION = 0x0001,
		COPY_ON_WRITE = 0x0002,
		SPACE_RESERVATION = 0x0004,
		ACCESS_CHECK = 0x0008
	};

	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			::Nirvana::Pointer (*allocate) (Bridge <::Nirvana::Memory>*, ::Nirvana::Pointer dst, ::Nirvana::UWord size, Flags flags, EnvironmentBridge*);
			void (*commit) (Bridge <::Nirvana::Memory>*, ::Nirvana::Pointer dst, ::Nirvana::UWord size, EnvironmentBridge*);
			void (*decommit) (Bridge <::Nirvana::Memory>*, ::Nirvana::Pointer dst, ::Nirvana::UWord size, EnvironmentBridge*);
			void (*release) (Bridge <::Nirvana::Memory>*, ::Nirvana::Pointer dst, ::Nirvana::UWord size, EnvironmentBridge*);
			::Nirvana::Pointer (*copy) (Bridge <::Nirvana::Memory>*, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, ::Nirvana::UWord size, Flags flags, EnvironmentBridge*);
			Boolean (*is_readable) (Bridge <::Nirvana::Memory>*, ::Nirvana::ConstPointer p, ::Nirvana::UWord size, EnvironmentBridge*);
			Boolean (*is_writable) (Bridge <::Nirvana::Memory>*, ::Nirvana::ConstPointer p, ::Nirvana::UWord size, EnvironmentBridge*);
			Boolean (*is_private) (Bridge <::Nirvana::Memory>*, ::Nirvana::ConstPointer p, ::Nirvana::UWord size, EnvironmentBridge*);
			Boolean (*is_copy) (Bridge <::Nirvana::Memory>*, ::Nirvana::ConstPointer p1, ::Nirvana::ConstPointer p2, ::Nirvana::UWord size, EnvironmentBridge*);
			::Nirvana::Word (*query) (Bridge <::Nirvana::Memory>*, ::Nirvana::ConstPointer p, QueryParam param, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal < ::Nirvana::Memory> (epv.interface)
	{}
};

template <class T>
class Client <T, ::Nirvana::Memory> :
	public T
{
public:
	::Nirvana::Pointer allocate (::Nirvana::Pointer dst, ::Nirvana::UWord size, Flags flags);
	void commit (::Nirvana::Pointer dst, ::Nirvana::UWord size);
	void decommit (::Nirvana::Pointer dst, ::Nirvana::UWord size);
	void release (::Nirvana::Pointer dst, ::Nirvana::UWord size);
	::Nirvana::Pointer copy (::Nirvana::Pointer dst, ::Nirvana::Pointer src, ::Nirvana::UWord size, Flags flags);
	Boolean is_readable (::Nirvana::ConstPointer p, ::Nirvana::UWord size);
	Boolean is_writable (::Nirvana::ConstPointer p, ::Nirvana::UWord size);
	Boolean is_private (::Nirvana::ConstPointer p, ::Nirvana::UWord size);
	Boolean is_copy (::Nirvana::ConstPointer p1, ::Nirvana::ConstPointer p2, ::Nirvana::UWord size);
	::Nirvana::Word query (::Nirvana::ConstPointer p, Bridge < ::Nirvana::Memory>::QueryParam param);
};

template <class T>
::Nirvana::Pointer Client <T, ::Nirvana::Memory>::allocate (::Nirvana::Pointer dst, ::Nirvana::UWord size, Flags flags)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	::Nirvana::Pointer _ret = (_b._epv ().epv.allocate) (&_b, dst, size, flags, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Nirvana::Memory>::commit (::Nirvana::Pointer dst, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.commit) (&_b, dst, size, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::Memory>::decommit (::Nirvana::Pointer dst, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.decommit) (&_b, dst, size, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::Memory>::release (::Nirvana::Pointer dst, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.release) (&_b, dst, size, &_env);
	_env.check ();
}

template <class T>
::Nirvana::Pointer Client <T, ::Nirvana::Memory>::copy (::Nirvana::Pointer dst, ::Nirvana::Pointer src, ::Nirvana::UWord size, Flags flags)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	::Nirvana::Pointer _ret = (_b._epv ().epv.copy) (&_b, dst, src, size, flags, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::Nirvana::Memory>::is_readable (::Nirvana::ConstPointer p, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.is_readable) (&_b, p, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::Nirvana::Memory>::is_writable (::Nirvana::ConstPointer p, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.is_writable) (&_b, p, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::Nirvana::Memory>::is_private (::Nirvana::ConstPointer p, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.is_private) (&_b, p, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::Nirvana::Memory>::is_copy (::Nirvana::ConstPointer p1, ::Nirvana::ConstPointer p2, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.is_copy) (&_b, p1, p2, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::Word Client <T, ::Nirvana::Memory>::query (::Nirvana::ConstPointer p, Bridge < ::Nirvana::Memory>::QueryParam param)
{
	Environment _env;
	Bridge < ::Nirvana::Memory>& _b (T::_get_bridge (_env));
	::Nirvana::Word _ret = (_b._epv ().epv.query) (&_b, p, param, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Memory : public ::CORBA::Nirvana::ClientInterface <Memory>
{};

}

#endif

