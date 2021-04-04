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
#ifndef NIRVANA_CURRENT_S_H_
#define NIRVANA_CURRENT_S_H_

#include "Current.h"
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Current>
{
public:
	static const typename Bridge < ::Nirvana::Current>::EPV epv_;

protected:
	static ::Nirvana::DeadlineTime _get_deadline (Bridge < ::Nirvana::Current>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).deadline ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::Current>::EPV Skeleton <S, ::Nirvana::Current>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Current>::repository_id_,
		S::template __duplicate < ::Nirvana::Current>,
		S::template __release < ::Nirvana::Current>
	},
	{ // epv
		S::_get_deadline
	}
};

/// Static implementation
template <class S>
class ServantStatic <S, ::Nirvana::Current> : public ImplementationPseudoStatic <S, ::Nirvana::Current>
{};

}
}

#endif
