#include "value.hpp"
#include <iostream>
using namespace libapt;
using namespace libapt::as;

Value::Value() : m_type(UNDEFINED), m_boolean(false),
m_float(0.0), m_number(0), m_string("")
{
}

void Value::FromFloat(const float n)
{
	m_type = FLOAT;
	m_float = n;
}

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

void Value::FromByte(const uint8_t num)
{
	m_type = INTEGER;
	m_number = num;
}

void Value::FromBoolean(const bool c)
{
	m_type = BOOLEAN;
	m_boolean = c;
}

void Value::FromString(const std::string& s)
{
	m_type = STRING;
	m_string = s;
}

float Value::ToFloat()
{
	switch (m_type)
	{
	case INTEGER:
		return m_number;
		break;
	case FLOAT:
		return m_float;
		break;
	default:
		return 0.0;
		break;
	}
}

bool Value::ToBoolean()
{
	switch (m_type)
	{
	case INTEGER:
		return (m_number>=1);
		break;
	case FLOAT:
		return (m_float>=1.0);
		break;
	default:
		return 0.0;
		break;
	}
}

std::string Value::ToString()
{
	return m_string;
}