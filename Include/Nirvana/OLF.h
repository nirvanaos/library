/// Object loadable file structures.
#ifndef NIRVANA_OLF_H_
#define NIRVANA_OLF_H_

#include <CORBA/ServantBase.h>
#include <CORBA/LocalObject.h>

#define OLF_BIND "olfbind"

#pragma section (OLF_BIND, read, execute)

namespace Nirvana {

enum OLF_Command
{
	OLF_IMPORT_INTERFACE = 1,
	OLF_EXPORT_INTERFACE = 2,
	OLF_EXPORT_OBJECT = 3,
	OLF_EXPORT_LOCAL = 4,
};

struct ExportInterface
{
	Word command;
	const char* name;
	CORBA::Nirvana::Bridge <CORBA::Nirvana::Interface>* itf;
};

struct ExportObject
{
	Word command;
	const char* name;
	CORBA::Nirvana::BridgeMarshal <PortableServer::ServantBase>* implementation;
	CORBA::Nirvana::Bridge <PortableServer::ServantBase>* core_object;
};

struct ExportLocal
{
	Word command;
	const char* name;
	CORBA::Nirvana::BridgeMarshal <CORBA::AbstractBase>* implementation;
	CORBA::Nirvana::Bridge <CORBA::LocalObject>* core_object;
};

struct ImportInterface
{
	Word command;
	const char* name;
	const char* interface_id;
	CORBA::Nirvana::Bridge <CORBA::Nirvana::Interface>* itf;
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
