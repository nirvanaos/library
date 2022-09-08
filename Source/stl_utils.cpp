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
#include <CORBA/CORBA.h>
#include <signal.h>
#include <Nirvana/System.h>
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

void StdContainer::remove_proxy () const NIRVANA_NOEXCEPT
{
	g_system->runtime_proxy_remove (this);
}

StdDebugIterator::StdDebugIterator () NIRVANA_NOEXCEPT
{}

StdDebugIterator::StdDebugIterator (const void* cont) :
	proxy_ (g_system->runtime_proxy_get (cont))
{}

StdDebugIterator::StdDebugIterator (const StdDebugIterator& src) :
	proxy_ (src.proxy_)
{}

StdDebugIterator& StdDebugIterator::operator = (const StdDebugIterator& src)
{
	proxy_ = src.proxy_;
	return *this;
}

StdDebugIterator::~StdDebugIterator () NIRVANA_NOEXCEPT
{}

const void* StdDebugIterator::container () const NIRVANA_NOEXCEPT
{
	if (proxy_)
		return proxy_->object ();
	else
		return nullptr;
}

}
