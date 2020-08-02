#include <Nirvana/StlUtils.h>
#include <stdexcept>

namespace Nirvana {

void StdExceptions::xout_of_range (const char* msg)
{
	throw std::out_of_range (msg);
}

void StdExceptions::xlength_error (const char* msg)
{
	throw std::length_error (msg);
}

}
