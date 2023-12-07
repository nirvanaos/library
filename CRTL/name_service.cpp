#include "name_service.h"

namespace CRTL {

CosNaming::NamingContextExt::_ref_type name_service ()
{
	return CosNaming::NamingContextExt::_narrow (CORBA::g_ORB->resolve_initial_references ("NameService"));
}

}
