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
#ifndef NIRVANA_RUNTIMESUPPORT_S_H_
#define NIRVANA_RUNTIMESUPPORT_S_H_

#include "RuntimeSupport.h"

namespace CORBA {
namespace Nirvana {

// RuntimeProxy

template <class S>
class Skeleton <S, ::Nirvana::RuntimeProxy>
{
public:
	static const typename Bridge < ::Nirvana::RuntimeProxy>::EPV epv_;

protected:
	static const void* _object (Bridge < ::Nirvana::RuntimeProxy>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).object ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::RuntimeProxy>::EPV Skeleton <S, ::Nirvana::RuntimeProxy>::epv_ = {
	{ // header
		Bridge < ::Nirvana::RuntimeProxy>::repository_id_,
		S::template __duplicate < ::Nirvana::RuntimeProxy>,
		S::template __release < ::Nirvana::RuntimeProxy>
	},
	{ // epv
		S::_object
	}
};

// RuntimeSupport

template <class S>
class Skeleton <S, ::Nirvana::RuntimeSupport>
{
public:
	static const typename Bridge < ::Nirvana::RuntimeSupport>::EPV epv_;

protected:
	static Interface* _runtime_proxy_get (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* obj, Interface* _env)
	{
		try {
			return TypeI < ::Nirvana::RuntimeProxy>::ret (S::_implementation (_b).runtime_proxy_get (obj));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _runtime_proxy_remove (Bridge < ::Nirvana::RuntimeSupport>* _b, const void* key, Interface* _env)
	{
		try {
			S::_implementation (_b).runtime_proxy_remove (key);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

template <class S>
const Bridge < ::Nirvana::RuntimeSupport>::EPV Skeleton <S, ::Nirvana::RuntimeSupport>::epv_ = {
	{ // header
		Bridge < ::Nirvana::RuntimeSupport>::repository_id_,
		S::template __duplicate < ::Nirvana::RuntimeSupport>,
		S::template __release < ::Nirvana::RuntimeSupport>
	},
	{ // epv
		S::_runtime_proxy_get,
		S::_runtime_proxy_remove
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Nirvana::RuntimeSupport> : public ImplementationPseudo <S, ::Nirvana::RuntimeSupport>
{};

// Static implementation

template <class S>
class ServantStatic <S, ::Nirvana::RuntimeSupport> : public ImplementationPseudoStatic <S, ::Nirvana::RuntimeSupport>
{};

}
}

#endif
