#pragma once

#include <stdio.h>

class Object
{
public:
	Object (const char* type) :
		type_ (type)
	{
		printf ("%s object constructed\n", type);
	}

	~Object ()
	{
		printf ("%s object destructed\n", type_);
	}

	bool check_ok () const
	{
		return type_ != nullptr;
	}

private:
	const char* type_;
};
