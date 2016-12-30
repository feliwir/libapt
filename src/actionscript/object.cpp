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

void Object::SetPlaystate(const PlayState ps)
{
	m_ps = ps;
	OnPlayStateChanged();
}
