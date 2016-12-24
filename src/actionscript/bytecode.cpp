#include "bytecode.hpp"
using namespace libapt;
using namespace libapt::as;


bool libapt::as::RequireAlign(const Action a)
{
	switch (a)
	{
	case DEFINEFUNCTION:
	case DEFINEFUNCTION2:
	case CONSTANTPOOL:
		return true;
	}

	return false;
}