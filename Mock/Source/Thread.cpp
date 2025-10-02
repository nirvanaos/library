/*
* Nirvana mock library.
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
#include <Mock/Thread.h>
#include <errno.h>
#include <system_error>

namespace Nirvana {
namespace Mock {

Thread::~Thread ()
{
	if (impl_)
		std::terminate ();
}

Thread& Thread::operator = (Thread&& rhs) noexcept
{
	if (impl_)
		std::terminate ();
	impl_ = rhs.impl_;
	rhs.impl_ = nullptr;
	return *this;
}

void Thread::start ()
{
	if (!(impl_->host_thread = host_Thread_create (thread_proc, impl_))) {
		delete impl_;
		throw std::system_error (ENOMEM, std::system_category ());
	}
}

void Thread::join ()
{
	if (impl_) {
		ImplBase* impl = impl_;
		impl_ = nullptr;
		int err = host_Thread_join (impl->host_thread);
		std::exception_ptr exception = std::move (impl->exception);
		delete impl;
		if (err)
			throw std::system_error (err, std::system_category ());
		if (exception)
			std::rethrow_exception (exception);
	} else
		throw std::invalid_argument ("Not joinable thread");
}

void Thread::thread_proc (void* param) noexcept
{
	ImplBase* impl = reinterpret_cast <ImplBase*> (param);
	try {
		impl->run ();
	} catch (...) {
		impl->exception = std::current_exception ();
	}
}

}
}