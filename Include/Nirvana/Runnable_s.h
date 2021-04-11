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
#ifndef NIRVANA_LEGACY_RUNNABLE_S_H_
#define NIRVANA_LEGACY_RUNNABLE_S_H_

#include "Runnable.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Legacy::Runnable>
{
public:
	static const typename Bridge < ::Nirvana::Legacy::Runnable>::EPV epv_;

protected:
	static void _run (Bridge < ::Nirvana::Legacy::Runnable>* _b, Interface* _env)
	{
		try {
			S::_implementation (_b).run ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

template <class S>
const Bridge < ::Nirvana::Legacy::Runnable>::EPV Skeleton <S, ::Nirvana::Legacy::Runnable>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Legacy::Runnable>::repository_id_,
		S::template __duplicate < ::Nirvana::Legacy::Runnable>,
		S::template __release < ::Nirvana::Legacy::Runnable>
	},
	{ // epv
		S::_run
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Nirvana::Legacy::Runnable> :
	public ImplementationPseudo <S, ::Nirvana::Legacy::Runnable>
{};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::Legacy::Runnable> :
	public ImplementationPseudoStatic <S, ::Nirvana::Legacy::Runnable>
{};

// POA - style (virtual) implementation

template <>
class ServantPOA < ::Nirvana::Legacy::Runnable> : 
	public ImplementationPseudoPOA < ::Nirvana::Legacy::Runnable>
{
public:
	virtual void run () = 0;
};

}
}

#endif
