#include <__external_threading>
#include <CORBA/CORBA.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/Module.h>

_LIBCPP_BEGIN_NAMESPACE_STD

int __libcpp_recursive_mutex_init (__libcpp_recursive_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_recursive_mutex_lock (__libcpp_recursive_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

bool __libcpp_recursive_mutex_trylock (__libcpp_recursive_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return true;
}

int __libcpp_recursive_mutex_unlock (__libcpp_recursive_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_recursive_mutex_destroy (__libcpp_recursive_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_mutex_lock (__libcpp_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

bool __libcpp_mutex_trylock (__libcpp_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_mutex_unlock (__libcpp_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_mutex_destroy (__libcpp_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_condvar_signal (__libcpp_condvar_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_condvar_broadcast (__libcpp_condvar_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_condvar_wait (__libcpp_condvar_t*, __libcpp_mutex_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_condvar_timedwait (__libcpp_condvar_t*, __libcpp_mutex_t*, __libcpp_timespec_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_condvar_destroy (__libcpp_condvar_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

//
// Execute once
//
int __libcpp_execute_once (__libcpp_exec_once_flag* control, void (*init_routine)())
{
	Nirvana::the_posix->once (*control, init_routine);
	return 0;
}

bool __libcpp_thread_id_equal (__libcpp_thread_id, __libcpp_thread_id)
{
	NIRVANA_UNREACHABLE_CODE ();
	return true;
}

bool __libcpp_thread_id_less (__libcpp_thread_id, __libcpp_thread_id)
{
	NIRVANA_UNREACHABLE_CODE ();
	return false;
}

bool __libcpp_thread_isnull (const __libcpp_thread_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return true;
}

int __libcpp_thread_create (__libcpp_thread_t*, void* (*__func)(void*), void* __arg)
{
	NIRVANA_UNREACHABLE_CODE ();
	return ENOTSUP;
}

__libcpp_thread_id __libcpp_thread_get_current_id ()
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

__libcpp_thread_id __libcpp_thread_get_id (const __libcpp_thread_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return 0;
}

int __libcpp_thread_join (__libcpp_thread_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return ENOTSUP;
}

int __libcpp_thread_detach(__libcpp_thread_t*)
{
	NIRVANA_UNREACHABLE_CODE ();
	return ENOTSUP;
}

void __libcpp_thread_yield ()
{
	Nirvana::the_posix->yield ();
}

void __libcpp_thread_sleep_for (const chrono::nanoseconds& ns)
{
	Nirvana::the_posix->sleep (ns.count () / 100);
}

//
// Thread local storage
//
int __libcpp_tls_create (__libcpp_tls_key* key, void (*deleter)(void*))
{
	int ret = EINVAL;
	try {
		*key = Nirvana::the_module->CS_alloc (deleter);
		ret = 0;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			ret = e;
	} catch (...) {
	}
	return ret;
}

void* __libcpp_tls_get (__libcpp_tls_key key)
{
	try {
		return Nirvana::the_module->CS_get (key);
	} catch (...) {}
	return nullptr;
}

int __libcpp_tls_set (__libcpp_tls_key key, void* val)
{
	int ret = EINVAL;
	try {
		Nirvana::the_module->CS_set (key, val);
		ret = 0;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			ret = e;
	} catch (...) {
	}
	return ret;
}

_LIBCPP_END_NAMESPACE_STD
