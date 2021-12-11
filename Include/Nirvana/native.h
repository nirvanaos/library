/// \file
/// \brief Nirvana native types
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
#ifndef NIRVANA_NATIVE_H_
#define NIRVANA_NATIVE_H_

#include <CORBA/TypeByVal.h>

namespace Nirvana {
// Native IDL types
typedef void* Pointer;
typedef const void* ConstPointer;
typedef size_t Size; ///< Memory size
typedef ptrdiff_t PtrDiff; ///< Pointers difference
typedef uintptr_t UIntPtr; ///< Integer to fit the pointer
}

namespace CORBA {
namespace Internal {

template <>
struct Type < ::Nirvana::Pointer> : TypeByVal < ::Nirvana::Pointer>
{};

template <>
struct Type < ::Nirvana::ConstPointer> : TypeByVal < ::Nirvana::ConstPointer>
{};

}
}

#endif