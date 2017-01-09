#include "object.hpp"
using namespace libapt;
using namespace libapt::as;

Value Object::GetProperty(const std::string & property)
{
	return m_properties[property];
}

void Object::SetProperty(const std::string & property, Value v)
{
	m_properties[property] = v;
}

Value Object::GetVariable(const std::string & name)
{
	return m_variables[name];
}

void Object::SetVariable(const std::string & name, Value v)
{
	m_variables[name] = v;
}

void Object::SetPlaystate(const PlayState ps)
{
	m_ps = ps;
	OnPlayStateChanged();
}

void Object::SetCurrentFrame(const uint32_t frame)
{
	m_cf = frame;
	OnFrameChanged();
}

void Object::NextFrame()
{
	++m_cf;
}
