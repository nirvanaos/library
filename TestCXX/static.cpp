#define TEST_NAME "static construictors"

#include "tdefs.h"
#include <stdio.h>

bool static1_ok, static2_ok;

class Static1
{
public:
	Static1 ()
	{
		printf ("Static1 constructed\n");
		static1_ok = true;
	}

	bool check_ok ()
	{
		return static1_ok;
	}
};

class Static2
{
public:
	Static2 ()
	{
		printf ("Static2 constructed\n");
		static2_ok = true;
	}

	bool check_ok ()
	{
		return static2_ok;
	}
};

Static2& get_static2 ()
{
	static Static2 obj;
	return obj;
}

Static1 static1;

void test_main ()
{
	CHECK (static1.check_ok ());
	CHECK (get_static2 ().check_ok ());
}
