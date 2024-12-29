// Nirvana project
// Tests
// MockMemory class

#ifndef NIRVANA_TEST_MOCKMEMORY_H_
#define NIRVANA_TEST_MOCKMEMORY_H_

#ifdef _MSC_VER
#define NIRVANA_MOCK_IMPORT __declspec(dllimport)
#else
#define NIRVANA_MOCK_IMPORT
#endif

namespace Nirvana {
namespace Test {

extern NIRVANA_MOCK_IMPORT size_t allocated_bytes ();

}
}

#endif
