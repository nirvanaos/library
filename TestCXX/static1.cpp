#define TEST_NAME "static constructors"

#include "tdefs.h"
#include "Object.h"

Object global1 ("Global 1");

Object& get_local_static ()
{
	static Object obj ("Local static");
	return obj;
}

extern Object global2;

//thread_local Object thr_object ("Thread local");

void test_main ()
{
	CHECK (global1.check_ok ());
	CHECK (global2.check_ok ());
	CHECK (get_local_static ().check_ok ());
//	CHECK (thr_object.check_ok ());
}
