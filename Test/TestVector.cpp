#include <Nirvana/basic_string.h>
#include <Nirvana/vector.h>
#include <gtest/gtest.h>
#include <Mock/TestMock.h>
#include <set>

namespace TestSTL {

using namespace std;

template <class T>
class TestVector :
	public ::Nirvana::Test::TestMock
{
protected:
	TestVector ()
	{}

	virtual ~TestVector ()
	{}
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

template <>
void generate (size_t cnt, bool* ptr)
{
	for (long v = 0; cnt; ++v, --cnt)
		*(ptr++) = (v % 2) != 0;
}

using VectorTypes = ::testing::Types <long, std::string, std::set <int>, bool>;
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

TYPED_TEST (TestVector, Append)
{
	static const size_t BLOCK_SIZE = 0x10000 / sizeof (TypeParam);
	static const size_t BLOCK_COUNT = 2;
	vector <TypeParam> src;
	src.resize (BLOCK_SIZE);
	{
		vector <TypeParam> dst;
		dst.reserve (BLOCK_COUNT * BLOCK_SIZE);
		for (size_t block = 0; block < BLOCK_COUNT; ++block) {
			dst.insert (dst.end (), src.cbegin (), src.cend ());
		}
		EXPECT_EQ (dst.size (), src.size () * BLOCK_COUNT);
		for (size_t block = 0; block < BLOCK_COUNT; ++block) {
			EXPECT_TRUE (equal (src.begin (), src.end (), dst.begin () + block * BLOCK_SIZE));
		}
	}
	{
		vector <TypeParam> dst;
		dst.reserve (BLOCK_COUNT * BLOCK_SIZE);
		for (size_t block = 0; block < BLOCK_COUNT; ++block) {
			dst.insert (dst.end (), (const TypeParam*)src.data (), (const TypeParam*)src.data () + src.size ());
		}
		EXPECT_EQ (dst.size (), src.size () * BLOCK_COUNT);
		for (size_t block = 0; block < BLOCK_COUNT; ++block) {
			EXPECT_TRUE (equal (src.begin (), src.end (), dst.begin () + block * BLOCK_SIZE));
		}
	}
}

}
