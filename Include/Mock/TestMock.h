#ifndef NIRVANA_TEST_TESTMOCK_H_
#define NIRVANA_TEST_TESTMOCK_H_

#include <gtest/gtest.h>
#include "MockMemory.h"

namespace Nirvana {
namespace Test {

class TestMock :
	public ::testing::Test
{
protected:
	TestMock ()
	{}

	virtual ~TestMock ()
	{}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp ()
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
		allocated_ = allocated_bytes ();
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
		if (!HasFatalFailure ()) {
			EXPECT_EQ (allocated_bytes (), allocated_);
		}
	}

private:
	size_t allocated_;
};

}
}

#endif
