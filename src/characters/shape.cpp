#include "shape.hpp"
#include "../displayobject.hpp"
#include "../util.hpp"
using namespace libapt;

void Shape::Parse(uint8_t *& iter)
{
	m_bounds = read<glm::vec4>(iter);
	m_geometryId = read<uint32_t>(iter);
}

void Shape::Update(const Transformation& t, std::shared_ptr<DisplayObject> dObj)
{
	if (m_geometry)
	{
		m_geometry->Draw(t,m_owner);
	}
}
