#include "object.hpp"
using namespace libapt;
using namespace libapt::as;

Object::Object() : m_ps(STOPPED), m_cf(0)
{
}

Value Object::GetProperty(const std::string & property)
{
	return m_properties[property];
}

void Object::SetProperty(const std::string& property, Value v)
{
	m_properties[property] = v;
	OnPropertyChanged(property);
}

std::map<std::string, Value>& Object::GetProperties()
{
	return m_properties;
}

Value Object::GetVariable(const std::string & name)
{
	return m_variables[name];
}

void Object::SetVariable(const std::string & name, Value v)
{
	m_variables[name] = v;
}

std::map<std::string, Value>& Object::GetVariables()
{
	return m_variables;
}

void Object::SetPlaystate(const PlayState ps)
{
	m_ps = ps;
	OnPlayStateChanged();
}

void Object::SetCurrentFrame(const uint32_t frame)
{
	m_cf = frame;
}

void Object::NextFrame()
{
	++m_cf;
}

void Object::OnPropertyChanged(const std::string & property)
{
}

void Object::OnPlayStateChanged()
{
}