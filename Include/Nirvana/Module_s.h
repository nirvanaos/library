/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_MODULE_S_H_
#define NIRVANA_MODULE_S_H_

#include "Module.h"
#include <CORBA/ImplementationPseudo.h>
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Module>
{
public:
	static const typename Bridge < ::Nirvana::Module>::EPV epv_;

protected:
	static ::Nirvana::ConstPointer _get_base_address (Bridge < ::Nirvana::Module>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).base_address ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::Module>::EPV Skeleton <S, ::Nirvana::Module>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Module>::repository_id_,
		S::template __duplicate < ::Nirvana::Module>,
		S::template __release < ::Nirvana::Module>
	},
	{ // epv
		S::_get_base_address
	}
};

/// Standard implementation
template <class S>
class Servant <S, ::Nirvana::Module> : public ImplementationPseudo <S, ::Nirvana::Module>
{};

/// Static implementation
template <class S>
class ServantStatic <S, ::Nirvana::Module> : public ImplementationPseudoStatic <S, ::Nirvana::Module>
{};

}
}

#endif
