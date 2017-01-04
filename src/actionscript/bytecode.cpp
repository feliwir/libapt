#include "bytecode.hpp"
using namespace libapt;
using namespace libapt::as;


bool libapt::as::RequireAlign(const Action a)
{
	switch (a)
	{
	case GOTOFRAME:
	case DEFINEFUNCTION:
	case DEFINEFUNCTION2:
	case CONSTANTPOOL:
	case BRANCHIFTRUE:
	case EA_PUSHSTRING:
	case EA_GETSTRINGVAR:
		return true;
	}

	return false;
}