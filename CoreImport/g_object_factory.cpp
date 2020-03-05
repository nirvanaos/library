#include <CORBA/ObjectFactory.h>
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Nirvana {

__declspec (allocate(OLF_BIND))
extern const ::Nirvana::ImportInterfaceT <ObjectFactory> g_object_factory = {
	::Nirvana::OLF_IMPORT_INTERFACE, "CORBA/Nirvana/g_object_factory", ObjectFactory::interface_id_
};

}
}
