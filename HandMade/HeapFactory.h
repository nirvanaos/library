/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_HEAPFACTORY_H_
#define NIRVANA_HEAPFACTORY_H_

namespace Nirvana {

class Memory;
typedef ::CORBA::Internal::I_ptr <Memory> Memory_ptr;
typedef ::CORBA::Internal::I_var <Memory> Memory_var;

class HeapFactory;
typedef ::CORBA::Internal::I_ptr <HeapFactory> HeapFactory_ptr;
typedef ::CORBA::Internal::I_var <HeapFactory> HeapFactory_var;

}

namespace CORBA {
namespace Internal {

NIRVANA_BRIDGE_BEGIN(::Nirvana::HeapFactory, NIRVANA_REPOSITORY_ID ("HeapFactory"))
	Interface* (*create) (Bridge <::Nirvana::HeapFactory>*, Interface*);
	Interface* (*create_with_granularity) (Bridge <::Nirvana::HeapFactory>*, ULong granularity, Interface*);
NIRVANA_BRIDGE_END()

template <class T>
class Client <T, ::Nirvana::HeapFactory> :
	public T
{
public:
	I_var < ::Nirvana::Memory> create ();
	I_var < ::Nirvana::Memory> create_with_granularity (ULong granularity);
};

template <class T>
I_var < ::Nirvana::Memory> Client <T, ::Nirvana::HeapFactory>::create ()
{
	Environment _env;
	Bridge < ::Nirvana::HeapFactory>& _b (T::_get_bridge (_env));
	I_ret < ::Nirvana::Memory> _ret = (_b._epv ().epv.create) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
I_var < ::Nirvana::Memory> Client <T, ::Nirvana::HeapFactory>::create_with_granularity (ULong granularity)
{
	Environment _env;
	Bridge < ::Nirvana::HeapFactory>& _b (T::_get_bridge (_env));
	I_ret < ::Nirvana::Memory> _ret = (_b._epv ().epv.create_with_granularity) (&_b, granularity, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class HeapFactory : public ::CORBA::Internal::ClientInterface <HeapFactory>
{};

}

#endif
