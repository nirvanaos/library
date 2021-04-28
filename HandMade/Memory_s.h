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
#ifndef NIRVANA_MEMORY_S_H_
#define NIRVANA_MEMORY_S_H_

#include "Memory.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Nirvana::Memory>
{
public:
	static const typename Bridge < ::Nirvana::Memory>::EPV epv_;

protected:
	static ::Nirvana::Pointer _allocate (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::Size size, UShort flags, Interface* _env)
	{
		try {
			return S::_implementation (_b).allocate (dst, size, flags);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _commit (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::Size size, Interface* _env)
	{
		try {
			S::_implementation (_b).commit (dst, size);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _decommit (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::Size size, Interface* _env)
	{
		try {
			S::_implementation (_b).decommit (dst, size);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _Memory_release (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::Size size, Interface* _env)
	{
		try {
			S::_implementation (_b).release (dst, size);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static ::Nirvana::Pointer _copy (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, ::Nirvana::Size size, UShort flags, Interface* _env)
	{
		try {
			return S::_implementation (_b).copy (dst, src, size, flags);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Boolean _is_readable (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::Size size, Interface* _env)
	{
		try {
			return S::_implementation (_b).is_readable (p, size);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Boolean _is_writable (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::Size size, Interface* _env)
	{
		try {
			return S::_implementation (_b).is_writable (p, size);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Boolean _is_private (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, ::Nirvana::Size size, Interface* _env)
	{
		try {
			return S::_implementation (_b).is_private (p, size);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Boolean _is_copy (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p1, ::Nirvana::ConstPointer p2, ::Nirvana::Size size, Interface* _env)
	{
		try {
			return S::_implementation (_b).is_copy (p1, p2, size);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ::Nirvana::UIntPtr _query (Bridge < ::Nirvana::Memory>* _b, ::Nirvana::ConstPointer p, Type <Definitions < ::Nirvana::Memory>::QueryParam>::ABI_in param, Interface* _env)
	{
		try {
			return S::_implementation (_b).query (p, Type <Definitions < ::Nirvana::Memory>::QueryParam>::in (param));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}
};

template <class S>
const Bridge < ::Nirvana::Memory>::EPV Skeleton <S, ::Nirvana::Memory>::epv_ = {
	{ // header
		Bridge < ::Nirvana::Memory>::repository_id_,
		S::template __duplicate < ::Nirvana::Memory>,
		S::template __release < ::Nirvana::Memory>
	},
	{ // epv
		S::_allocate,
		S::_commit,
		S::_decommit,
		S::_Memory_release,
		S::_copy,
		S::_is_readable,
		S::_is_writable,
		S::_is_private,
		S::_is_copy,
		S::_query
	}
};

/// Standard implementation of the Memory interface
template <class S>
class Servant <S, ::Nirvana::Memory> : public ImplementationPseudo <S, ::Nirvana::Memory>
{};

/// Static implementation of the Memory interface
template <class S>
class ServantStatic <S, ::Nirvana::Memory> : public ImplementationPseudoStatic <S, ::Nirvana::Memory>
{};

}
}

#endif
