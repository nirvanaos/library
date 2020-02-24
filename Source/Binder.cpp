#include <Nirvana/Binder.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge < ::Nirvana::Binder>::interface_id_ [] = CORBA_NIRVANA_REPOSITORY_ID (Binder);

}
}
