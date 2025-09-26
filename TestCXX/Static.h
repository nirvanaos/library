#pragma once

#include <stdio.h>

class Static
{
public:
	Static ()
	{
		printf ("Static object constructed\n");
		ok_ = true;
	}

	~Static ()
	{
		printf ("Static object destructed\n");
	}

	bool check_ok () const
	{
		return ok_;
	}

private:
	bool ok_;
};
