#include "value.hpp"
#include <iostream>
using namespace libapt;
using namespace libapt::as;

void Value::FromConstant(const Const::Entry& e)
{
	switch (e.type)
	{
	case Const::STRING:
		m_type = STRING;
		m_string = e.stringVal;
		break;
	case Const::NUMBER:
		m_type = INTEGER;
		m_number = e.numVal;
		break;
	default:
		std::cout << "Unable to create as::Value from const entry" << std::endl;
	}
}
