#include <Nirvana/Nirvana.h>
#include <gtest/gtest.h>
#include <Nirvana/bitutils.h>
#include <Nirvana/Hash.h>
#include <CORBA/Server.h>
#include <Nirvana/Runnable_s.h>
#include <Mock/TestMock.h>
#include <functional>

namespace TestLibrary {

using namespace Nirvana;

class TestLibrary :
	public Nirvana::Test::TestMock
{
	typedef ::Nirvana::Test::TestMock Base;
protected:
	TestLibrary ()
	{}

	virtual ~TestLibrary ()
	{}
};

TEST_F (TestLibrary, NLZ)
{
	static const uint32_t test [] = {0,32, 1,31, 2,30, 3,30, 4,29, 5,29, 6,29,
		7,29, 8,28, 9,28, 16,27, 32,26, 64,25, 128,24, 255,24, 256,23,
		512,22, 1024,21, 2048,20, 4096,19, 8192,18, 16384,17, 32768,16,
		65536,15, 0x20000,14, 0x40000,13, 0x80000,12, 0x100000,11,
		0x200000,10, 0x400000,9, 0x800000,8, 0x1000000,7, 0x2000000,6,
		0x4000000,5, 0x8000000,4, 0x0FFFFFFF,4, 0x10000000,3,
		0x3000FFFF,2, 0x50003333,1, 0x7FFFFFFF,1, 0x80000000,0,
		0xFFFFFFFF,0};

	for (size_t i = 0; i < countof (test); i += 2) {
		EXPECT_EQ (nlz (test [i]), test [i + 1]) << "x = " << std::hex << test [i];;
	}

	for (size_t i = 0; i < countof (test); i += 2) {
		EXPECT_EQ (nlz ((uint64_t)test [i]), test [i + 1] + 32) << "x = " << std::hex << test [i];;
	}

	for (size_t i = 0; i < countof (test); i += 2) {
		EXPECT_EQ (nlz ((uint64_t)test [i] << 32), test [i] ? test [i + 1] : 64) << "x = " << std::hex << test [i];;
	}
}

TEST_F (TestLibrary, NTZ)
{
	static uint32_t test [] = {0, 32, 1,0, 2,1, 3,0, 4,2, 5,0, 6,1,  7,0,
		8,3, 9,0, 16,4, 32,5, 64,6, 128,7, 255,0, 256,8, 512,9, 1024,10,
		2048,11, 4096,12, 8192,13, 16384,14, 32768,15, 65536,16,
		0x20000,17, 0x40000,18, 0x80000,19, 0x100000,20, 0x200000,21,
		0x400000,22, 0x800000,23, 0x1000000,24, 0x2000000,25,
		0x4000000,26, 0x8000000,27, 0x10000000,28, 0x20000000,29,
		0x40000000,30, 0x80000000,31, 0xFFFFFFF0,4, 0x3000FF00,8,
		0xC0000000,30, 0x60000000,29, 0x00011000, 12};

	for (size_t i = 0; i < countof (test); i += 2) {
		EXPECT_EQ (ntz (test [i]), test [i + 1]) << "x = " << std::hex << test [i];
	}

	for (size_t i = 0; i < countof (test); i += 2) {
		EXPECT_EQ (ntz ((uint64_t)test [i]), test [i] ? test [i + 1] : 64) << "x = " << std::hex << test [i];
	}

	for (size_t i = 0; i < countof (test); i += 2) {
		EXPECT_EQ (ntz ((uint64_t)test [i] << 32), test [i + 1] + 32) << "x = " << std::hex << test [i];
	}
}

TEST_F (TestLibrary, FLP2)
{
	static const size_t test [] = {0,0, 1,1, 2,2, 3,2, 4,4, 5,4, 7,4, 8,8,
		9,8, 15,8, 16,16, 0xffff,0x8000, 0x7fffffff,0x40000000,
		0x80000000,0x80000000, 0x80000001,0x80000000,
		0xffffffff,0x80000000};

	for (size_t i = 0; i < countof (test); i += 2) {
		EXPECT_EQ (flp2 (test [i]), test [i + 1]) << "x=" << std::hex << test [i];
	}
}

TEST_F (TestLibrary, CLP2)
{
	static const size_t test [] = {0,0, 1,1, 2,2, 3,4, 4,4, 5,8, 7,8, 8,8,
		9,16, 15,16, 16,16, 0xffff,0x10000, 0x7fffffff,0x80000000,
		0x80000000,0x80000000, 0x80000001, (size_t)0x100000000,
		0xffffffff, (size_t)0x100000000};

	for (size_t i = 0; i < countof (test); i += 2) {
		EXPECT_EQ (clp2 (test [i]), test [i + 1]) << "x=" << std::hex << test [i];
	}
}

TEST_F (TestLibrary, Log2)
{
	static const unsigned l2_1 = log2_ceil (1);
	static const unsigned l2_2 = log2_ceil (2);
	static const unsigned l2_3 = log2_ceil (3);
	static const unsigned l2_4 = log2_ceil (4);
	static const unsigned l2_5 = log2_ceil (5);
	EXPECT_EQ (l2_1, 0);
	EXPECT_EQ (l2_2, 1);
	EXPECT_EQ (l2_3, 2);
	EXPECT_EQ (l2_4, 2);
	EXPECT_EQ (l2_5, 3);
}

TEST_F (TestLibrary, ILog2)
{
	EXPECT_EQ (ilog2_ceil (1u), 0);
	EXPECT_EQ (ilog2_ceil (2u), 1);
	EXPECT_EQ (ilog2_ceil (3u), 2);
	EXPECT_EQ (ilog2_ceil (4u), 2);
	EXPECT_EQ (ilog2_ceil (5u), 3);

	EXPECT_EQ (ilog2_floor (1u), 0);
	EXPECT_EQ (ilog2_floor (2u), 1);
	EXPECT_EQ (ilog2_floor (3u), 1);
	EXPECT_EQ (ilog2_floor (4u), 2);
	EXPECT_EQ (ilog2_floor (5u), 2);
}

TEST_F (TestLibrary, Hash)
{
	EXPECT_NE (Hash::hash_bytes ("aaaa", 4), Hash::hash_bytes ("bbbbb", 5));
}

class Functor :
	public CORBA::Internal::Servant <Functor, ::Nirvana::Legacy::Runnable>,
	public CORBA::Internal::LifeCycleStatic
{
public:
	Functor (const std::function <void ()>& f) :
		func_ (f)
	{}

	void run ()
	{
		func_ ();
	}

private:
	std::function <void ()> func_;
};

TEST_F (TestLibrary, Runnable)
{
	using namespace std;

	int a = 1, b = 2, c = 0;

	Functor functor ([a, b, &c]() { c = a + b; });
	::Nirvana::Legacy::Runnable::_ptr_type r = functor._get_ptr ();
	r->run ();
	EXPECT_EQ (c, a + b);
}

}
