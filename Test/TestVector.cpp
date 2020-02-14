#include <Nirvana/basic_string.h>
#include <Nirvana/vector.h>
#include <set>
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

template <>
void generate (size_t cnt, set <int>* ptr)
{
	for (int v = 0; cnt; ++v, --cnt) {
		set <int>& s = *(ptr++);
		for (int i = v, cnt = 3; cnt; ++i, --cnt) {
			s.insert (i);
		}
	}
}

using VectorTypes = ::testing::Types <long, std::string, std::set <int> >;
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
		EXPECT_EQ (vec.size (), size (v));
		EXPECT_TRUE (equal (vec.begin (), vec.end (), v));
		vector <TypeParam> vec1 (vec);
		EXPECT_EQ (vec1.size (), size (v));
		EXPECT_TRUE (equal (vec1.begin (), vec1.end (), v));
		vector <TypeParam> vec2 (move (vec1));
		EXPECT_EQ (vec2.size (), size (v));
		EXPECT_TRUE (equal (vec2.begin (), vec2.end (), v));
	}
}

TYPED_TEST (TestVector, Reserve)
{
	TypeParam v [10];
	generate (size (v), v);
	vector <TypeParam> vec (v, v + size (v));
	vec.reserve (20);
	EXPECT_EQ (vec.size (), size (v));
	EXPECT_TRUE (equal (vec.begin (), vec.end (), v));
	EXPECT_GE (vec.capacity (), 20U);
}

TYPED_TEST (TestVector, PushBack)
{
	TypeParam v [10];
	generate (size (v), v);
	vector <TypeParam> vec;
	for (const TypeParam& rv : v) {
		vec.push_back (rv);
	}
	EXPECT_EQ (vec.size (), size (v));
	EXPECT_TRUE (equal (vec.begin (), vec.end (), v));
}

}

