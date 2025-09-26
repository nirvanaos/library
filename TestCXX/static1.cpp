#define TEST_NAME "static constructors"

#include "tdefs.h"
#include "Static.h"

Static& get_local_static ()
{
	static Static obj;
	return obj;
}

Static global1;

extern Static global2;

void test_main ()
{
	CHECK (global1.check_ok ());
	CHECK (global2.check_ok ());
	CHECK (get_local_static ().check_ok ());
}
