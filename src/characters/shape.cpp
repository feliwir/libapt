#include "shape.hpp"
#include "../displayobject.hpp"
#include "../util.hpp"
#include <iostream>
using namespace libapt;

void Shape::Parse(uint8_t *& iter)
{
	m_bounds = read<glm::vec4>(iter);
	m_geometryId = read<uint32_t>(iter);
}

void Shape::Update(const Transformation& t, std::shared_ptr<DisplayObject> dObj)
{
	assert(m_geometry);

	if (t.visible)
	{
		m_geometry->Draw(t,m_owner);
	}
	else
	{
	}
}
