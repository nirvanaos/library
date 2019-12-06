#include <Nirvana/basic_string.h>
#include <Nirvana/vector.h>
#include <gtest/gtest.h>

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

template <class T>
void generate (size_t cnt, T* ptr);

template <>
void generate (size_t cnt, long* ptr)
{
	for (long v = 0; cnt; ++v, --cnt)
		*(ptr++) = v;
}

template <>
void generate (size_t cnt, string* ptr)
{
	for (long v = 0; cnt; ++v, --cnt)
		*(ptr++) = to_string (v);
}

using VectorTypes = ::testing::Types <long, std::string>;
TYPED_TEST_SUITE (TestVector, VectorTypes);

TYPED_TEST (TestVector, Constructor)
{
	{
		vector <TypeParam> vec;
	}
	{
		TypeParam v;
		generate (1, &v);
		vector <TypeParam> vec (10, v);
	}
	{
		TypeParam v[10];
		generate (size(v), v);
		vector <TypeParam> vec (v, v + size(v));
		vector <TypeParam> vec1 (vec);
		vector <TypeParam> vec2 (move (vec1));
	}
}

}

