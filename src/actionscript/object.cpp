#include "object.hpp"
#include <algorithm>
using namespace libapt;
using namespace libapt::as;

Object::Object() : m_ps(PLAYING), m_cf(0)
{
}

Value Object::GetProperty(const std::string & property)
{
	std::string p = property;
	std::transform(p.begin(), p.end(), p.begin(), ::tolower);
	if (m_properties.find(p) == m_properties.end())
		return Value();

	return m_properties[p];
}

void Object::SetProperty(const std::string& property, Value v)
{
	std::string p = property;
	std::transform(p.begin(), p.end(), p.begin(), ::tolower);
	m_properties[p] = v;
	OnPropertyChanged(p);
}

std::map<std::string, Value>& Object::GetProperties()
{
	return m_properties;
}

void Object::SetPlaystate(const PlayState ps)
{
	m_ps = ps;
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
