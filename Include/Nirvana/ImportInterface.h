#ifndef NIRVANA_IMPORTINTERFACE_H_
#define NIRVANA_IMPORTINTERFACE_H_

#include "NirvanaBase.h"
#include <CORBA/I_ptr.h>

namespace Nirvana {

struct ImportInterface
{
	Word command;
	const char* name;
	const char* interface_id;
	CORBA::Nirvana::Interface* itf;
};

template <class I>
struct ImportInterfaceT
{
	ImportInterface imp;

	operator CORBA::Nirvana::I_ptr <I> () const
	{
		return static_cast <I*> (imp.itf);
	}

	I* operator -> () const
	{
		return static_cast <I*> (imp.itf);
	}
};

}

#endif
