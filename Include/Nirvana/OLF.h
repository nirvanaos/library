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
	CORBA::Nirvana::Interface* implementation;
	CORBA::Nirvana::Bridge <::PortableServer::ServantBase>* core_object;
};

struct ExportLocal
{
	Word command;
	const char* name;
	CORBA::Nirvana::Interface* implementation;
	CORBA::Nirvana::Bridge <CORBA::Object>* core_object;
};

}

#if defined _M_AMD64
#define C_NAME_PREFIX ""
#else
#define C_NAME_PREFIX "_"
#endif

#define NIRVANA_EXPORT_INTERFACE1(ns, name, servant, itf)\
__pragma(comment(linker, "/include:" C_NAME_PREFIX "_exp_"#ns"_"#name))\
extern "C" __declspec (allocate(OLF_BIND))\
const Nirvana::ExportInterface _exp_##ns##_##name {Nirvana::OLF_EXPORT_INTERFACE, #ns "/" #name, STATIC_BRIDGE (servant, itf)};

#define NIRVANA_EXPORT_INTERFACE2(ns1, ns2, name, servant, itf)\
__pragma(comment(linker, "/include:" C_NAME_PREFIX "_exp_"#ns1"_"#ns2"_"#name))\
extern "C" __declspec (allocate(OLF_BIND))\
const Nirvana::ExportInterface _exp_##ns1##_##ns2##_##name {Nirvana::OLF_EXPORT_INTERFACE, #ns1 "/" #ns2 "/" #name, STATIC_BRIDGE (servant, itf)};

#endif
