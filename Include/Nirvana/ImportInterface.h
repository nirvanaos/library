#ifndef NIRVANA_IMPORTINTERFACE_H_
#define NIRVANA_IMPORTINTERFACE_H_

#include "NirvanaBase.h"
#include <CORBA/I_ptr.h>

namespace Nirvana {

enum OLF_Command : Word
{
	OLF_END = 0,
	OLF_IMPORT_INTERFACE = 1,
	OLF_EXPORT_INTERFACE = 2,
	OLF_EXPORT_OBJECT = 3,
	OLF_EXPORT_LOCAL = 4,
};

struct ImportInterface
{
	OLF_Command command;
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
		assert (imp.itf);
		return static_cast <I*> (imp.itf);
	}

	I* operator -> () const
	{
		assert (imp.itf);
		return static_cast <I*> (imp.itf);
	}
};

}

#endif
