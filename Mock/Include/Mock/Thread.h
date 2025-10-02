/*
* Nirvana mock library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#ifndef NIRVANA_MOCK_THREAD_H_
#define NIRVANA_MOCK_THREAD_H_
#pragma once

#include <functional>

#include <mockhost/HostAPI.h>

namespace Nirvana {
namespace Mock {

/// Drop-in replacement for std::thread
class Thread
{
public:
  Thread () noexcept :
    impl_ (nullptr)
  {}

  Thread (Thread&& src) noexcept :
    impl_ (src.impl_)
  {
    src.impl_ = nullptr;
  }

  template <class F, class ... Args>
  Thread (F&& f, Args&& ... args) :
    Thread (std::bind (std::forward <F> (f), std::forward <Args> (args)...))
  {}

  ~Thread ();

	Thread& operator = (Thread&& rhs) noexcept;

  bool joinable () const noexcept
  {
    return impl_ != nullptr;
  }

	void join ();

	void swap (Thread& other) noexcept
	{
		std::swap (impl_, other.impl_);
	}

  static unsigned int hardware_concurrency () noexcept
  {
    return host_hardware_concurrency ();
  }

private:
	template <class F>
	Thread (F&& f) :
		impl_ (new Impl <F> (std::move (f)))
	{
		start ();
	}

  static void thread_proc (void* param) noexcept;
	void start ();

	struct ImplBase
	{
		void operator delete (void* p) noexcept
		{
			host_release (p);
		}

		ImplBase () :
			host_thread (nullptr)
		{}

		virtual ~ImplBase ()
		{}

		virtual void run () = 0;

		std::exception_ptr exception;
		host_Thread* host_thread;
	};

	template <class F>
	struct Impl : ImplBase
	{
		void* operator new (size_t cb)
		{
			return host_allocate (cb, alignof (Impl));
		}

		Impl (F&& f) :
			function (std::move (f))
		{}

		void run () override
		{
			function ();
		}

		F function;
	};

  ImplBase* impl_;
};

}
}

#endif
