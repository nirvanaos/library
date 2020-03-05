#ifndef NIRVANA_STATELESS_H_
#define NIRVANA_STATELESS_H_

#include <CORBA/core_objects.h>
#include <utility>

namespace Nirvana {

template <class T, class ... Args>
NIRVANA_NODISCARD T* stateless_create (Args ... args)
{
	int tmp [(sizeof (T) + sizeof (int) - 1) / sizeof (int)];
	CORBA::Nirvana::StatelessCreationFrame scb { tmp, sizeof (T) };
	CORBA::Nirvana::g_object_factory->stateless_begin (scb);
	try {
		new (tmp) T (std::forward <Args> (args)...);
		return (T*)CORBA::Nirvana::g_object_factory->stateless_end (true);
	} catch (...) {
		CORBA::Nirvana::g_object_factory->stateless_end (false);
		throw;
	}
}

inline const void* stateless_copy (const void* src, size_t size)
{
	return CORBA::Nirvana::g_object_factory->stateless_copy (src, size);
}

inline void stateless_free (const void* src, size_t size)
{
	CORBA::Nirvana::g_object_factory->memory_release (const_cast <void*> (src), size);
}

}

#endif
