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
#ifndef NIRVANA_LEGACY_PROCESS_S_H_
#define NIRVANA_LEGACY_PROCESS_S_H_

#include "Process.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Legacy::Process>
{
public:
	static const typename Bridge < ::Nirvana::Legacy::Process>::EPV epv_;

protected:
	static Long _main (Bridge < ::Nirvana::Legacy::Process>* _b, UShort argc, Char** argv, Interface* _env)
	{
		try {
			return S::_implementation (_b).main (argc, argv);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

template <class S>
const Bridge < ::Nirvana::Legacy::Process>::EPV Skeleton <S, ::Nirvana::Legacy::Process>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Legacy::Process>::repository_id_,
		S::template __duplicate < ::Nirvana::Legacy::Process>,
		S::template __release < ::Nirvana::Legacy::Process>
	},
	{ // epv
		S::_main
	}
};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::Legacy::Process> :
	public ImplementationPseudoStatic <S, ::Nirvana::Legacy::Process>
{};

}
}

#endif
