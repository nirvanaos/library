#include <Nirvana/Nirvana.h>
#include <gtest/gtest.h>
#include <Mock/TestMock.h>

namespace TestSTL {

template <class S>
class TestString : public Nirvana::Test::TestMock
{
protected:
	TestString ()
	{}

	virtual ~TestString ()
	{}
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

template <class S>
bool operator == (const S& s, const Const <S>& c)
{
	return s.length () == c.length () && std::equal (s.data (), s.data () + s.length (), 
		static_cast <const typename S::value_type*> (c));
}

using StringTypes = ::testing::Types <std::basic_string <char>, std::basic_string <CORBA::WChar> >;
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
		{
			TypeParam s (cs);
			invariants (s);
			EXPECT_EQ (s, cs);
		}
		{
			TypeParam s (cs, cs.length ());
			invariants (s);
			EXPECT_EQ (s, cs);
		}
	}
	{
		Const <TypeParam> cs ("large string large string very large string");
		{
			TypeParam s (cs);
			invariants (s);
			EXPECT_EQ (s, cs);

			// Test for range-based loop
			for (typename TypeParam::value_type& val : s) {
				val = 'c';
			}
		}
		{
			TypeParam s (cs, cs.length ());
			invariants (s);
			EXPECT_EQ (s, cs);
		}
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

TYPED_TEST (TestString, Iterators)
{
	TypeParam s (Const <TypeParam> ("large string large string very large string"));
	TypeParam s1;
	for (typename TypeParam::const_iterator it = s.cbegin (); it != s.cend (); ++it) {
		s1.push_back (*it);
	}
	EXPECT_EQ (s, s1);
}

#ifdef NIRVANA_C11

TYPED_TEST (TestString, initializer_list)
{
	TypeParam s = {'1', '2', '3'};
	EXPECT_EQ (s, Const <TypeParam> ("123"));
}

#endif

#ifdef NIRVANA_C17

TYPED_TEST (TestString, string_view)
{
	TypeParam s (Const <TypeParam> ("string"));
	typedef std::basic_string_view <typename TypeParam::value_type, typename TypeParam::traits_type> View;
	View v = s;
	s = v;
}

#endif

TYPED_TEST (TestString, Resize)
{
	TypeParam s;
	s.resize (20);
	EXPECT_EQ (s.size (), 20u);
}

TYPED_TEST (TestString, ShrinkExpand)
{
	TypeParam s (Const <TypeParam> ("string string string string string "));
	EXPECT_EQ (s.size (), 35);
	size_t au = (size_t)Nirvana::the_memory->query (s.c_str (), Nirvana::Memory::QueryParam::ALLOCATION_UNIT);
	EXPECT_EQ (s.capacity (), (((s.size () + 1) * sizeof (typename TypeParam::value_type) + au - 1) / au * au) / sizeof (typename TypeParam::value_type) - 1);
	s.resize (23); // Must be > SMALL_CAPACITY for all platforms
	EXPECT_EQ (s.size (), 23);
	s.shrink_to_fit ();
	EXPECT_EQ (s.capacity (), (((s.size () + 1) * sizeof (typename TypeParam::value_type) + au - 1) / au * au) / sizeof (typename TypeParam::value_type) - 1);
	s.reserve (35);
	EXPECT_EQ (s.capacity (), (((35 + 1) * sizeof (typename TypeParam::value_type) + au - 1) / au * au) / sizeof (typename TypeParam::value_type) - 1);
}

TYPED_TEST (TestString, Insert)
{
	TypeParam s (Const <TypeParam> ("/"));
	s.insert (0, 1, '\\');
	EXPECT_EQ (s, TypeParam (Const <TypeParam> ("\\/")));
}

}
