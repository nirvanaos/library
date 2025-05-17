#include <Nirvana/Nirvana.h>
#include <gtest/gtest.h>
#include <set>
#include <iterator>

namespace TestSTL {

template <class T>
class TestVector : public ::testing::Test
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
void generate (size_t cnt, std::string* ptr)
{
	for (long v = 0; cnt; ++v, --cnt)
		*(ptr++) = std::to_string (v);
}

template <>
void generate (size_t cnt, std::set <int>* ptr)
{
	for (int v = 0; cnt; ++v, --cnt) {
		std::set <int>& s = *(ptr++);
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
		std::vector <TypeParam> vec;
	}
	{
		TypeParam v;
		generate (1, &v);
		std::vector <TypeParam> vec (10, v);

		// Test for range-based loop
		for (typename std::vector <TypeParam>::reference val : vec) {
			val = TypeParam ();
		}
	}
	{
		TypeParam v[10];
		generate (countof(v), v);
		std::vector <TypeParam> vec (v, std::end (v));
		EXPECT_EQ (vec.size (), countof (v));
		EXPECT_TRUE (std::equal (vec.begin (), vec.end (), v));
		std::vector <TypeParam> vec1 (vec);
		EXPECT_EQ (vec1.size (), countof (v));
		EXPECT_TRUE (std::equal (vec1.begin (), vec1.end (), v));
		std::vector <TypeParam> vec2 (std::move (vec1));
		EXPECT_EQ (vec2.size (), countof (v));
		EXPECT_TRUE (std::equal (vec2.begin (), vec2.end (), v));
	}
}

TYPED_TEST (TestVector, Reserve)
{
	TypeParam v [10];
	generate (countof (v), v);
	std::vector <TypeParam> vec (v, std::end (v));
	vec.reserve (20);
	EXPECT_EQ (vec.size (), countof (v));
	EXPECT_TRUE (std::equal (vec.begin (), vec.end (), v));
	EXPECT_GE (vec.capacity (), 20U);
}

TYPED_TEST (TestVector, PushBack)
{
	TypeParam v [10];
	generate (countof (v), v);
	std::vector <TypeParam> vec;
	for (const TypeParam& rv : v) {
		vec.push_back (rv);
	}
	EXPECT_EQ (vec.size (), countof (v));
	EXPECT_TRUE (std::equal (vec.begin (), vec.end (), v));
}

TYPED_TEST (TestVector, Append)
{
	static const size_t BLOCK_SIZE = 0x10000 / sizeof (TypeParam);
	static const size_t BLOCK_COUNT = 2;
	std::vector <TypeParam> src;
	src.resize (BLOCK_SIZE);
	{
		std::vector <TypeParam> dst;
		dst.reserve (BLOCK_COUNT * BLOCK_SIZE);
		for (size_t block = 0; block < BLOCK_COUNT; ++block) {
			dst.insert (dst.end (), src.cbegin (), src.cend ());
		}
		EXPECT_EQ (dst.size (), src.size () * BLOCK_COUNT);
		for (size_t block = 0; block < BLOCK_COUNT; ++block) {
			EXPECT_TRUE (std::equal (src.begin (), src.end (), dst.begin () + block * BLOCK_SIZE));
		}
	}
	{
		std::vector <TypeParam> dst;
		dst.reserve (BLOCK_COUNT * BLOCK_SIZE);
		for (size_t block = 0; block < BLOCK_COUNT; ++block) {
			dst.insert (dst.end (), &*src.begin (), &*src.begin () + src.size ());
		}
		EXPECT_EQ (dst.size (), src.size () * BLOCK_COUNT);
		for (size_t block = 0; block < BLOCK_COUNT; ++block) {
			EXPECT_TRUE (std::equal (src.begin (), src.end (), dst.begin () + block * BLOCK_SIZE));
		}
	}
}

}
