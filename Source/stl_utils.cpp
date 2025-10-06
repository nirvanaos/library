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
#include <Nirvana/stl_utils.h>
#include <Nirvana/Debugger.h>
#include <stdexcept>

namespace Nirvana {

void StdExceptions::xout_of_range (const char* msg)
{
	throw std::out_of_range (msg);
}

void StdExceptions::xlength_error (const char* msg)
{
	throw std::length_error (msg);
}

CORBA::Internal::I_ref <RuntimeProxy> StdDebugIterator::get_proxy (const void* cont)
{
	return the_debugger->proxy_get (cont);
}

void StdContainer::remove_proxy () const noexcept
{
	try {
		the_debugger->proxy_reset (this);
	} catch (...) {}
}

StdDebugIterator::StdDebugIterator () noexcept
{}

StdDebugIterator::StdDebugIterator (const StdDebugIterator& src) :
	proxy_ (src.proxy_)
{}

StdDebugIterator& StdDebugIterator::operator = (const StdDebugIterator& src)
{
	proxy_ = src.proxy_;
	return *this;
}

StdDebugIterator::~StdDebugIterator () noexcept
{}

const void* StdDebugIterator::container () const noexcept
{
	if (proxy_)
		return proxy_->object ();
	else
		return nullptr;
}

}
