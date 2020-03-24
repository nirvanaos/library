/// Object loadable file structures.
#ifndef NIRVANA_OLF_H_
#define NIRVANA_OLF_H_

#include "ImportInterface.h"
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
	CORBA::Nirvana::Interface* itf;
};

struct ExportObject
{
	Word command;
	const char* name;
	CORBA::Nirvana::Interface* servant_base;
	CORBA::Nirvana::Bridge <::PortableServer::ServantBase>* core_object;
};

struct ExportLocal
{
	Word command;
	const char* name;
	CORBA::Nirvana::Interface* local_object;
	CORBA::Nirvana::Interface* abstract_base;
	CORBA::Nirvana::Bridge <CORBA::Object>* core_object;
};

}

#if defined _M_AMD64
#define C_NAME_PREFIX ""
#else
#define C_NAME_PREFIX "_"
#endif

#define NIRVANA_EXPORT_INTERFACE1(ns, name, Servant, Itf)\
__pragma(comment(linker, "/include:" C_NAME_PREFIX "_exp_"#ns"_"#Servant))\
extern "C" __declspec (allocate(OLF_BIND))\
const Nirvana::ExportInterface _exp_##ns##_##Servant {Nirvana::OLF_EXPORT_INTERFACE, name, STATIC_BRIDGE (ns::Servant, Itf)}

#define NIRVANA_EXPORT_INTERFACE2(ns1, ns2, name, Servant, Itf)\
__pragma(comment(linker, "/include:" C_NAME_PREFIX "_exp_"#ns1"_"#ns2"_"#Servant))\
extern "C" __declspec (allocate(OLF_BIND))\
const Nirvana::ExportInterface _exp_##ns1##_ns2##_##Servant {Nirvana::OLF_EXPORT_INTERFACE, name, STATIC_BRIDGE (ns1::ns2::Servant, Itf)}

#endif
