#include <Nirvana/basic_string.h>
#include <gtest/gtest.h>
#include <CORBA/ObjectFactory_c.h>
#include <Mock/MockMemory.h>

namespace TestSTL {

using namespace std;

template <class S>
class TestString :
	public ::testing::Test
{
protected:
	TestString ()
	{}

	virtual ~TestString ()
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

template <class S>
class Const
{
public:
	Const (const char* s)
	{
		len_ = strlen (s);
		size_t cc = len_ + 1;
		s_ = new typename S::value_type [cc];
		std::copy (s, s + cc, s_);
	}

	~Const ()
	{
		delete [] s_;
	}

	operator const typename S::value_type* () const
	{
		return s_;
	}

	size_t length () const
	{
		return len_;
	}

private:
	typename S::value_type* s_;
	size_t len_;
};

using StringTypes = ::testing::Types <std::basic_string <char>, std::basic_string <wchar_t> >;
TYPED_TEST_SUITE (TestString, StringTypes);

template <class S>
void invariants (const S& s)
{
	EXPECT_LE (s.length (), s.capacity ());
	EXPECT_EQ (s.c_str () [s.length ()], 0);
}

TYPED_TEST (TestString, Constructor)
{
	{
		Const <TypeParam> cs ("smal");
		TypeParam s (cs);
		invariants (s);
		EXPECT_EQ (s.length (), cs.length ());
		EXPECT_STREQ (s.c_str (), cs);
	}
	{
		Const <TypeParam> cs ("large string large string very large string");
		TypeParam s (cs);
		invariants (s);
		EXPECT_EQ (s.length (), cs.length ());
		EXPECT_STREQ (s.c_str (), cs);
	}
}

TYPED_TEST (TestString, find)
{
	TypeParam s (Const <TypeParam> ("large string large string very large string"));
	Const <TypeParam> fs ("string");
	EXPECT_EQ (s.find (fs), 6);
	EXPECT_EQ (s.find (TypeParam (fs)), 6);
	EXPECT_EQ (s.rfind (fs), s.length () - 6);
	EXPECT_EQ (s.rfind (TypeParam (fs)), s.length () - 6);
	EXPECT_EQ (s.find ('a'), 1);
	EXPECT_EQ (s.rfind ('i'), s.length () - 3);
	EXPECT_EQ (s.find (Const <TypeParam> ("not found")), TypeParam::npos);
	EXPECT_EQ (s.find ('\n'), TypeParam::npos);
}

#if __cplusplus >= 201103L

TYPED_TEST (TestString, initializer_list)
{
	TypeParam s = {'1', '2', '3'};
	EXPECT_STREQ (s.c_str (), Const <TypeParam> ("123"));
}

#endif

#if __cplusplus >= 201703L

TYPED_TEST (TestString, string_view)
{
	TypeParam s (Const <TypeParam> ("string"));
	typedef basic_string_view <typename TypeParam::value_type, typename TypeParam::traits_type> View;
	View v = s;
	s = v;
}

#endif

}

