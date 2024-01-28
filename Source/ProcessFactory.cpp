// This file was generated from "ProcessFactory.idl"
// Nirvana IDL to C++ compiler version 0.0.1
#include "../../pch/pch.h"
#include <CORBA/CORBA.h>
#include <Nirvana/OLF.h>
#include "../Include/Nirvana/ProcessFactory.h"

NIRVANA_OLF_SECTION_OPT extern const Nirvana::ImportInterfaceT <CORBA::TypeCode>
Nirvana::_tc_InheritedFile = { Nirvana::OLF_IMPORT_INTERFACE, CORBA::Internal::RepIdOf <Nirvana::InheritedFile>::id, CORBA::Internal::RepIdOf <CORBA::TypeCode>::id }; 

namespace CORBA {
namespace Internal {

void Type < ::Nirvana::InheritedFile>::check (const ABI& val)
{
	Type < ::Nirvana::Access>::check (val._access);
	Type < ::IDL::Sequence <CORBA::UShort> >::check (val._descriptors);
}

void Type < ::Nirvana::InheritedFile>::marshal_in (const Var& v, IORequest::_ptr_type rq)
{
	Type < ::Nirvana::Access>::marshal_in (v._access, rq);
	Type < ::IDL::Sequence <CORBA::UShort> >::marshal_in (v._descriptors, rq);
}

void Type < ::Nirvana::InheritedFile>::marshal_out (Var& v, IORequest::_ptr_type rq)
{
	Type < ::Nirvana::Access>::marshal_out (v._access, rq);
	Type < ::IDL::Sequence <CORBA::UShort> >::marshal_out (v._descriptors, rq);
}

void Type < ::Nirvana::InheritedFile>::unmarshal (IORequest::_ptr_type rq, Var& v)
{
	Type < ::Nirvana::Access>::unmarshal (rq, v._access);
	Type < ::IDL::Sequence <CORBA::UShort> >::unmarshal (rq, v._descriptors);
}

template <>
class TypeDef <&::Nirvana::_tc_InheritedFiles> :
	public TypeCodeTypeDef <&::Nirvana::_tc_InheritedFiles, ::IDL::Sequence < ::Nirvana::InheritedFile> >
{};

}
}

namespace Nirvana {

extern NIRVANA_SELECTANY
const ::CORBA::Internal::Alias _tc_InheritedFiles { "IDL:Nirvana/InheritedFiles:1.0", "InheritedFiles",
NIRVANA_STATIC_BRIDGE (::CORBA::TypeCode, ::CORBA::Internal::TypeDef <&_tc_InheritedFiles>) }; 

}

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Nirvana::Process>::name_ [] = "Process";

}
}

NIRVANA_SELECTANY const CORBA::Internal::StaticTC
Nirvana::_tc_Process = { NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, CORBA::Internal::TypeCodeInterface <Nirvana::Process>) };

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Nirvana::AMI_ProcessPoller>::name_ [] = "AMI_ProcessPoller";

}
}

NIRVANA_SELECTANY const CORBA::Internal::StaticTC
Nirvana::_tc_AMI_ProcessPoller = { NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, CORBA::Internal::TypeCodeValueAbstract <Nirvana::AMI_ProcessPoller>) };

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Nirvana::AMI_ProcessHandler>::name_ [] = "AMI_ProcessHandler";

}
}

NIRVANA_SELECTANY const CORBA::Internal::StaticTC
Nirvana::_tc_AMI_ProcessHandler = { NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, CORBA::Internal::TypeCodeInterface <Nirvana::AMI_ProcessHandler>) };

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Nirvana::ProcessCallback>::name_ [] = "ProcessCallback";

}
}

NIRVANA_SELECTANY const CORBA::Internal::StaticTC
Nirvana::_tc_ProcessCallback = { NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, CORBA::Internal::TypeCodeInterface <Nirvana::ProcessCallback>) };

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Nirvana::AMI_ProcessCallbackPoller>::name_ [] = "AMI_ProcessCallbackPoller";

}
}

NIRVANA_SELECTANY const CORBA::Internal::StaticTC
Nirvana::_tc_AMI_ProcessCallbackPoller = { NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, CORBA::Internal::TypeCodeValueAbstract <Nirvana::AMI_ProcessCallbackPoller>) };

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Nirvana::AMI_ProcessCallbackHandler>::name_ [] = "AMI_ProcessCallbackHandler";

}
}

NIRVANA_SELECTANY const CORBA::Internal::StaticTC
Nirvana::_tc_AMI_ProcessCallbackHandler = { NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, CORBA::Internal::TypeCodeInterface <Nirvana::AMI_ProcessCallbackHandler>) };

namespace CORBA {
namespace Internal {

template <>
class TypeDef <&Decls < ::Nirvana::ProcessFactory>::_tc_StringSeq> :
	public TypeCodeTypeDef <&Decls < ::Nirvana::ProcessFactory>::_tc_StringSeq, ::IDL::Sequence < ::IDL::String> >
{};
NIRVANA_SELECTANY
const Alias Decls < ::Nirvana::ProcessFactory>::_tc_StringSeq { "IDL:Nirvana/ProcessFactory/StringSeq:1.0", "StringSeq",
NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, TypeDef <&Decls < ::Nirvana::ProcessFactory>::_tc_StringSeq>) }; 

template <>
const Char TypeCodeName < ::Nirvana::ProcessFactory>::name_ [] = "ProcessFactory";

}
}

NIRVANA_SELECTANY const CORBA::Internal::StaticTC
Nirvana::_tc_ProcessFactory = { NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, CORBA::Internal::TypeCodeInterface <Nirvana::ProcessFactory>) };

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Nirvana::AMI_ProcessFactoryPoller>::name_ [] = "AMI_ProcessFactoryPoller";

}
}

NIRVANA_SELECTANY const CORBA::Internal::StaticTC
Nirvana::_tc_AMI_ProcessFactoryPoller = { NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, CORBA::Internal::TypeCodeValueAbstract <Nirvana::AMI_ProcessFactoryPoller>) };

namespace CORBA {
namespace Internal {

template <>
const Char TypeCodeName < ::Nirvana::AMI_ProcessFactoryHandler>::name_ [] = "AMI_ProcessFactoryHandler";

}
}

NIRVANA_SELECTANY const CORBA::Internal::StaticTC
Nirvana::_tc_AMI_ProcessFactoryHandler = { NIRVANA_STATIC_BRIDGE (CORBA::TypeCode, CORBA::Internal::TypeCodeInterface <Nirvana::AMI_ProcessFactoryHandler>) };

NIRVANA_OLF_SECTION_OPT extern const Nirvana::ImportInterfaceT <Nirvana::ProcessFactory>
Nirvana::g_launcher = { Nirvana::OLF_IMPORT_OBJECT, "Nirvana/g_launcher", CORBA::Internal::RepIdOf <Nirvana::ProcessFactory>::id };
