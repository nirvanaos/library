#include <Nirvana/Nirvana.h>
#include <gtest/gtest.h>
#include <Nirvana/bitutils.h>
#include <Nirvana/Hash.h>
#include <Nirvana/string_conv.h>
#include <Nirvana/strtoi.h>
#include <Nirvana/strtof.h>
#include <Nirvana/Formatter.h>
#include <Nirvana/Parser.h>
#include <random>

namespace TestLibrary {

using namespace Nirvana;

class TestLibrary : public ::testing::Test
{
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
	EXPECT_EQ (l2_1, 0u);
	EXPECT_EQ (l2_2, 1u);
	EXPECT_EQ (l2_3, 2u);
	EXPECT_EQ (l2_4, 2u);
	EXPECT_EQ (l2_5, 3u);
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

#define STR_ENTRY(s) { (const char*)s, countof(s) - 1 }

TEST_F (TestLibrary, UTF8)
{
	const struct Str
	{
		const char* s;
		size_t len;
	} strings [] = {
		STR_ENTRY (u8"Here's a string in english"),
		STR_ENTRY (u8"أنا أفضل أن أكون هنا"),
		STR_ENTRY (u8"Je préfère d'être ici. On ne voit bien qu'avec le cœur."),
		STR_ENTRY (u8"لعَرَبِيةُ中原音韵")
	};

	for (size_t i = 0; i < countof (strings); ++i) {
		EXPECT_TRUE (is_valid_utf8 (strings [i].s, strings [i].len)) << "string# " << i;

		std::wstring w;
		append_wide (strings [i].s, strings [i].s + strings [i].len, w);
		std::string s;
		append_utf8 (w, s);
		int cmp = s.compare ((size_t)0, s.size (), strings [i].s, strings [i].len);
		EXPECT_EQ (cmp, 0) << "string# " << i;
	}
}

TEST_F (TestLibrary, Exception)
{
	std::exception_ptr ep;
	EXPECT_FALSE (ep);

	try {
		throw std::runtime_error ("Test");
	} catch (...) {
		ep = std::current_exception ();
	}

	ASSERT_TRUE (ep);

	bool ok = false;
	try {
		std::rethrow_exception (ep);
	} catch (const std::runtime_error& ex) {
		ok = !strcmp (ex.what (), "Test");
	}

	EXPECT_TRUE (ok);
}

TEST_F (TestLibrary, StrToI)
{
	struct Test
	{
		const char* s;
		unsigned base;
		int end;
		uint32_t u32;
		int err_u32;
		int32_t i32;
		int err_i32;
	};

	static const Test tests [] = {
		// s,          base, end,   uint32_t,  error,    int32_t,  error
		{ "",             0,   0,          0, EINVAL,          0, EINVAL },
		{ "0xFFFFFFFF",   0,  -1, 0xFFFFFFFF,      0,         -1,      0 },
		{ "0xFFFFFFFFF",  0,  -1, 0xFFFFFFFF, ERANGE, 0x7FFFFFFF, ERANGE },
		{ "0",            0,  -1,          0,      0,          0,      0 },
		{ "0x",           0,   0,          0, EINVAL,          0, EINVAL },
		{ "0x",          10,   1,          0,      0,          0,      0 }
	};

	for (size_t i = 0; i < std::size (tests); ++i) {
		const Test& test = tests [i];
		char* end;

		uint32_t u32;
		strtoi (test.s, &end, test.base, u32);
		EXPECT_EQ (u32, test.u32) << i;
		EXPECT_EQ (errno, test.err_u32) << i;
		if (test.end < 0)
			EXPECT_EQ (end, test.s + strlen (test.s)) << i;
		else
			EXPECT_EQ (end, test.s + test.end) << i;

		int32_t i32;
		strtoi (test.s, &end, test.base, i32);
		EXPECT_EQ (i32, test.i32) << i;
		EXPECT_EQ (errno, test.err_i32) << i;
		if (test.end < 0)
			EXPECT_EQ (end, test.s + strlen (test.s)) << i;
		else
			EXPECT_EQ (end, test.s + test.end) << i;
	}
}

TEST_F (TestLibrary, Formatter)
{
	std::string s;
	size_t cnt = Formatter::append_format (s, "%d %d", 1, -1);
	EXPECT_EQ (cnt, s.size ());
	EXPECT_EQ (s, "1 -1");
	s.clear ();

	cnt = Formatter::append_format (s, "%e", 12345.6789);
	EXPECT_EQ (cnt, s.size ());
	EXPECT_EQ (s, "1.234568e+04");
	s.clear ();

	cnt = Formatter::append_format (s, "%e", -12345.6789);
	EXPECT_EQ (cnt, s.size ());
	EXPECT_EQ (s, "-1.234568e+04");
	s.clear ();

	cnt = Formatter::append_format (s, "%f", 12345.6789);
	EXPECT_EQ (cnt, s.size ());
	EXPECT_EQ (s, "12345.678900");
	s.clear ();

	cnt = Formatter::append_format (s, "%f", -12345.6789);
	EXPECT_EQ (cnt, s.size ());
	EXPECT_EQ (s, "-12345.678900");
	s.clear ();
}

TEST_F (TestLibrary, FormatterF)
{
	std::string s;
	size_t cnt;
	long double ld0, ld;

	ld0 = -std::numeric_limits <long double>::max ();
	cnt = Formatter::append_format (s, "%.0Lf", ld0);
	EXPECT_EQ (cnt, s.size ());
	strtof (s.c_str (), (char**)nullptr, ld);
	if (sizeof (ld0) == 8) {
		const char test [] =
			"-179769313"
			"486231570814527"
			"423731704356798"
			"070567525844996"
			"598917476803157"
			"260780028538760"
			"589558632766878"
			"171540458953514"
			"382464234321326"
			"889464182768467"
			"546703537516986"
			"049910576551282"
			"076245490090389"
			"328944075868508"
			"455133942304583"
			"236903222948165"
			"808559332123348"
			"274797826204144"
			"723168738177180"
			"919299881250404"
			"026184124858368";
		EXPECT_EQ (s.length (), strlen (test));
		EXPECT_EQ (s, test);
	}

	EXPECT_EQ (errno, 0);
	EXPECT_DOUBLE_EQ (ld, ld0);
	s.clear ();

	ld0 = std::numeric_limits <long double>::max ();
	cnt = Formatter::append_format (s, "%.0Lf", ld0);
	EXPECT_EQ (cnt, s.size ());
	strtof (s.c_str (), (char**)nullptr, ld);
	EXPECT_EQ (errno, 0);
	EXPECT_DOUBLE_EQ (ld, ld0);
	s.clear ();

	ld0 = 0;
	cnt = Formatter::append_format (s, "%.0Lf", ld0);
	EXPECT_EQ (cnt, s.size ());
	strtof (s.c_str (), (char**)nullptr, ld);
	EXPECT_EQ (errno, 0);
	EXPECT_EQ (ld, ld0);
	s.clear ();

	std::uniform_real_distribution <long double> dist (0, std::numeric_limits <long double>::max ());
	std::mt19937 gen;
	for (int i = 0; i < 100000; ++i) {
		ld0 = dist (gen);
		cnt = Formatter::append_format (s, "%.*Lf", std::numeric_limits <long double>::digits10, ld0);
		EXPECT_EQ (cnt, s.size ());
		strtof (s.c_str (), (char**)nullptr, ld);
		EXPECT_EQ (errno, 0);
		EXPECT_DOUBLE_EQ (ld, ld0);
		s.clear ();
	}
}

TEST_F (TestLibrary, FormatterE)
{
	std::string s;
	size_t cnt;
	long double ld = 0;
	cnt = Formatter::append_format (s, "%Le", ld);
	EXPECT_EQ (cnt, s.size ());
	EXPECT_EQ (s, "0.000000e+00");
	s.clear ();

	std::uniform_real_distribution <long double> dist (0, std::numeric_limits <long double>::max ());
	std::mt19937 gen;
	for (int i = 0; i < 100000; ++i) {
		ld = dist (gen);
		if (ld > 0) {
			cnt = Formatter::append_format (s, "%Le", ld);
			EXPECT_EQ (cnt, s.size ()) << i;
			const char* ss = s.c_str ();
			EXPECT_GT (ss [0], '0') << i;
			EXPECT_EQ (ss [1], '.') << i;
			EXPECT_EQ (ss [8], 'e') << i;
			EXPECT_TRUE (ss [9] == '+' || ss [9] == '-') << i;
			s.clear ();
		}
	}
}

TEST_F (TestLibrary, FormatterA)
{
	std::string s;
	size_t cnt;
	long double ld0, ld;

	ld0 = -std::numeric_limits <long double>::max ();
	cnt = Formatter::append_format (s, "%La", ld0);
	EXPECT_EQ (cnt, s.size ());
	if (sizeof (ld0) == 8)
		EXPECT_EQ (s, "-0x1.fffffffffffffp+1024");
	strtof (s.c_str (), (char**)nullptr, ld);
	EXPECT_EQ (errno, 0);
	EXPECT_EQ (ld, ld0);
	s.clear ();

	ld0 = std::numeric_limits <long double>::max ();
	cnt = Formatter::append_format (s, "%La", ld0);
	EXPECT_EQ (cnt, s.size ());
	if (sizeof (ld0) == 8)
		EXPECT_EQ (s, "0x1.fffffffffffffp+1024");
	strtof (s.c_str (), (char**)nullptr, ld);
	EXPECT_EQ (errno, 0);
	EXPECT_EQ (ld, ld0);
	s.clear ();

	ld0 = 0;
	cnt = Formatter::append_format (s, "%La", ld0);
	EXPECT_EQ (cnt, s.size ());
	strtof (s.c_str (), (char**)nullptr, ld);
	if (sizeof (ld0) == 8)
		EXPECT_EQ (s, "0x0.0000000000000p+00");
	EXPECT_EQ (errno, 0);
	EXPECT_EQ (ld, ld0);
	s.clear ();

	std::uniform_real_distribution <long double> dist (0, std::numeric_limits <long double>::max ());
	std::mt19937 gen;
	for (int i = 0; i < 100000; ++i) {
		ld0 = dist (gen);
		cnt = Formatter::append_format (s, "%La", ld0);
		EXPECT_EQ (cnt, s.size ());
		strtof (s.c_str (), (char**)nullptr, ld);
		EXPECT_EQ (errno, 0);
		ASSERT_EQ (ld, ld0) << i << ' ' << s;
		s.clear ();
	}
}

TEST_F (TestLibrary, StrToF)
{
	struct Test
	{
		const char* s;
		float f;
		int err_f;
		double d;
		int err_d;
		long double l;
		int err_l;
	};

	static const Test tests [] = {
		{ "1", 1, 0, 1, 0, 1, 0 },
		{ "+1.0", 1, 0, 1, 0, 1, 0 },
		{ "inf",
			std::numeric_limits <float>::infinity (), 0,
			std::numeric_limits <double>::infinity (), 0,
			std::numeric_limits <long double>::infinity (), 0 },
		{ "+inf",
			std::numeric_limits <float>::infinity (), 0,
			std::numeric_limits <double>::infinity (), 0,
			std::numeric_limits <long double>::infinity (), 0 },
		{ "-inf",
			-std::numeric_limits <float>::infinity (), 0,
			-std::numeric_limits <double>::infinity (), 0,
			-std::numeric_limits <long double>::infinity (), 0 },
		{ "infinity",
			std::numeric_limits <float>::infinity (), 0,
			std::numeric_limits <double>::infinity (), 0,
			std::numeric_limits <long double>::infinity (), 0 },
		{ "+infinity",
			std::numeric_limits <float>::infinity (), 0,
			std::numeric_limits <double>::infinity (), 0,
			std::numeric_limits <long double>::infinity (), 0 },
		{ "-infinity",
			-std::numeric_limits <float>::infinity (), 0,
			-std::numeric_limits <double>::infinity (), 0,
			-std::numeric_limits <long double>::infinity (), 0 },
	};

	for (size_t i = 0; i < std::size (tests); ++i) {
		const Test& test = tests [i];
		float f;
		char* end;

		strtof (test.s, &end, f);
		EXPECT_EQ (f, test.f) << i;
		EXPECT_EQ (errno, test.err_f) << i;
		EXPECT_EQ (end, test.s + strlen (test.s)) << i;

		if (sizeof (double) > sizeof (float)) {
			double d;

			strtof (test.s, &end, d);
			EXPECT_EQ (d, test.d) << i;
			EXPECT_EQ (errno, test.err_d) << i;
			EXPECT_EQ (end, test.s + strlen (test.s)) << i;

			if (sizeof (long double) > sizeof (double)) {
				long double l;

				strtof (test.s, &end, l);
				EXPECT_EQ (l, test.l) << i;
				EXPECT_EQ (errno, test.err_l) << i;
				EXPECT_EQ (end, test.s + strlen (test.s)) << i;
			}
		}
	}

	static const char* const nans [] = {
		"NaN", "nan", "NAN", "+nan", "-nan"
	};

	for (auto s : nans) {
		float f;
		char* end;

		strtof (s, &end, f);
		EXPECT_TRUE (std::isnan (f));
		EXPECT_EQ (errno, 0);
		EXPECT_EQ (end, s + strlen (s));

		if (sizeof (double) > sizeof (float)) {
			double d;

			strtof (s, &end, d);
			EXPECT_TRUE (std::isnan (d));
			EXPECT_EQ (errno, 0);
			EXPECT_EQ (end, s + strlen (s));

			if (sizeof (long double) > sizeof (double)) {
				long double l;

				strtof (s, &end, l);
				EXPECT_TRUE (std::isnan (l));
				EXPECT_EQ (errno, 0);
				EXPECT_EQ (end, s + strlen (s));
			}
		}
	}
}

TEST_F (TestLibrary, Scan)
{
	int i = 0;
	float f = 0;
	size_t cnt = Parser::parse ("10 3.5", "%i %f", &i, &f);
	EXPECT_EQ (cnt, 2);
	EXPECT_EQ (i, 10);
	EXPECT_EQ (f, 3.5);
}

}
