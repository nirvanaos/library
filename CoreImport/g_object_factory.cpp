#include <CORBA/ObjectFactory.h>
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Nirvana {

NIRVANA_OLF_SECTION
extern const ::Nirvana::ImportInterfaceT <ObjectFactory> g_object_factory = {
	::Nirvana::OLF_IMPORT_INTERFACE, "CORBA/Nirvana/g_object_factory", ObjectFactory::repository_id_
};

}
}
