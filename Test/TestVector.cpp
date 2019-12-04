#include <Nirvana/basic_string.h>
#include <Nirvana/vector.h>
#include <gtest/gtest.h>
//#include <Mock/MockMemory.h>

namespace TestSTL {

using namespace std;

template <class T>
class TestVector :
	public ::testing::Test
{
protected:
	TestVector ()
	{}

	virtual ~TestVector ()
	{}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp ()
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}
};

using VectorTypes = ::testing::Types <long, std::basic_string <char> >;
TYPED_TEST_SUITE (TestVector, VectorTypes);

TYPED_TEST (TestVector, Constructor)
{
	std::vector <TypeParam> vec;
}

}

