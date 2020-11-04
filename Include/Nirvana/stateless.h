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

}

#endif
