#include <Nirvana/basic_string.h>
#include <gtest/gtest.h>
#include <CORBA/ObjectFactory_c.h>
#include <Mock/MockMemory.h>

namespace CORBA {
namespace Nirvana {
Bridge <ObjectFactory>* const ObjectFactory::singleton_ = nullptr;
}
}

namespace TestSTL {

using namespace std;

template <typename Char>
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

template <typename Char>
class Const
{
public:
	Const (const char* s)
	{
		len_ = strlen (s);
		size_t cc = len_ + 1;
		s_ = new Char [cc];
		copy (s, s + cc, s_);
	}

	~Const ()
	{
		delete [] s_;
	}

	operator const Char* () const
	{
		return s_;
	}

	size_t length () const
	{
		return len_;
	}

private:
	Char* s_;
	size_t len_;
};

using CharTypes = ::testing::Types <wchar_t, char>;
TYPED_TEST_SUITE (TestString, CharTypes);

TYPED_TEST (TestString, Constructor)
{
	{
		Const <TypeParam> cs ("smal");
		basic_string <TypeParam> s (cs);
		EXPECT_EQ (s.length (), cs.length ());
		EXPECT_LE (s.length (), s.capacity ());
		EXPECT_EQ (s.c_str () [s.length ()], 0);
		EXPECT_STREQ (s.c_str (), cs);
	}
	{
		Const <TypeParam> cs ("large string large string very large string");
		basic_string <TypeParam> s (cs);
		EXPECT_EQ (s.length (), cs.length ());
		EXPECT_LE (s.length (), s.capacity ());
		EXPECT_EQ (s.c_str () [s.length ()], 0);
		EXPECT_STREQ (s.c_str (), cs);
	}
}

}

