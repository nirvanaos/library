/*
* Nirvana mock library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/

#ifndef NIRVANA_MOCK_TEST_H_
#define NIRVANA_MOCK_TEST_H_
#pragma once

#include <gtest/gtest.h>
#include "Memory.h"

namespace Nirvana {
namespace Mock {

class Test :
	public ::testing::Test
{
protected:
	Test ()
	{}

	virtual ~Test ()
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
			ptrdiff_t leaks = allocated_bytes () - allocated_;
			EXPECT_EQ (leaks, 0);
		}
	}

private:
	size_t allocated_;
};

}
}

#endif
