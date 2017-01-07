#include "shape.hpp"
#include "../displayobject.hpp"
#include "../util.hpp"
using namespace libapt;

void Shape::Parse(uint8_t *& iter)
{
	m_bounds = read<glm::vec4>(iter);
	m_geometryId = read<uint32_t>(iter);
}

void Shape::Update(const Transformation& t, DisplayObject& dObj)
{
	if(m_geometry)
		m_geometry->Draw(t);
}
