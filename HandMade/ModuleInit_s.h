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
#ifndef NIRVANA_MODULEINIT_S_H_
#define NIRVANA_MODULEINIT_S_H_

#include "ModuleInit.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::ModuleInit>
{
public:
	static const typename Bridge < ::Nirvana::ModuleInit>::EPV epv_;

protected:
	static void _initialize (Bridge < ::Nirvana::ModuleInit>* _b, Interface* _env)
	{
		try {
			S::_implementation (_b).initialize ();
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _terminate (Bridge < ::Nirvana::ModuleInit>* _b, Interface* _env)
	{
		try {
			S::_implementation (_b).terminate ();
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::ModuleInit>::EPV Skeleton <S, ::Nirvana::ModuleInit>::epv_ = {
	{ // header
		Bridge < ::Nirvana::ModuleInit>::repository_id_,
		S::template __duplicate < ::Nirvana::ModuleInit>,
		S::template __release < ::Nirvana::ModuleInit>
	},
	{ // epv
		S::_initialize,
		S::_terminate
	}
};

/// Static implementation
template <class S>
class ServantStatic <S, ::Nirvana::Module> : public ImplementationPseudoStatic <S, ::Nirvana::Module>
{};

}
}

#endif
