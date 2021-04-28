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
#ifndef NIRVANA_BINDER_S_H_
#define NIRVANA_BINDER_S_H_

#include "Binder.h"
#include "CORBA/ImplementationPseudo.h"
#include "CORBA/ImplementationPseudoStatic.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Binder>
{
public:
	static const typename Bridge <::Nirvana::Binder>::EPV epv_;

protected:
	static Interface* _bind (Bridge <::Nirvana::Binder>* obj, Type <String>::ABI_in name, Type <String>::ABI_in interface_id, Interface* env)
	{
		try {
			return Type <Interface>::ret (S::_implementation (obj).bind (Type <String>::in (name), Type <String>::in (interface_id)));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}
};

template <class S>
const Bridge <::Nirvana::Binder>::EPV Skeleton <S, ::Nirvana::Binder>::epv_ = {
	{ // header
		Bridge <::Nirvana::Binder>::repository_id_,
		S::template __duplicate <::Nirvana::Binder>,
		S::template __release <::Nirvana::Binder>
	},
	{ // epv
		S::_bind
	}
};

template <class S>
class Servant <S, ::Nirvana::Binder> :
	public ImplementationPseudo <S, ::Nirvana::Binder>
{};

template <class S>
class ServantStatic <S, ::Nirvana::Binder> :
	public ImplementationPseudoStatic <S, ::Nirvana::Binder>
{};

}
}

#endif
