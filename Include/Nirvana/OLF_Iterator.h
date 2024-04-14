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
#ifndef NIRVANA_OLF_ITERATOR_H_
#define NIRVANA_OLF_ITERATOR_H_
#pragma once

#include "OLF.h"

namespace Nirvana {

template <typename Word = uintptr_t>
class OLF_Iterator
{
public:
	OLF_Iterator (const void* address, size_t size) :
		cur_ptr_ ((Word*)address),
		end_ ((Word*)((uint8_t*)address + size))
	{
		check ();
	}

	bool end () const noexcept
	{
		return cur_ptr_ == end_;
	}

	Word* cur () const noexcept
	{
		return cur_ptr_;
	}

	void next ()
	{
		if (!end ()) {
			size_t idx = (size_t)(*cur_ptr_) - 1;
			assert (idx >= 0);
			cur_ptr_ = (Word*)((uint8_t*)cur_ptr_ + command_sizes_ [idx]);
			check ();
		}
	}

private:
	void check ();

private:
	Word* cur_ptr_;
	Word* end_;

	static const size_t command_sizes_ [OLF_MODULE_STARTUP];
};

template <typename Word>
const size_t OLF_Iterator <Word>::command_sizes_ [OLF_MODULE_STARTUP] = {
	sizeof (ImportInterfaceW <Word>),
	sizeof (ExportInterfaceW <Word>),
	sizeof (ExportObjectW <Word>),
	sizeof (ExportLocalW <Word>),
	sizeof (ImportInterfaceW <Word>),
	sizeof (ModuleStartupW <Word>)
};

template <typename Word>
void OLF_Iterator <Word>::check ()
{
	if (cur_ptr_ >= end_)
		cur_ptr_ = end_;
	else {
		Word cmd = *cur_ptr_;
		if (OLF_END == cmd)
			cur_ptr_ = end_;
		else if ((size_t)cmd > countof (command_sizes_))
			throw_INTERNAL (make_minor_errno (ENOEXEC));
	}
}

}

#endif
