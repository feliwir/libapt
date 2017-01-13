#include "button.hpp"
#include "../util.hpp"
#include <libapt/apt.hpp>
using namespace libapt;

void Button::Parse(uint8_t *& iter)
{
	//reserved flags
	m_unknown = read<uint32_t>(iter);
	m_bounds = read<glm::vec4>(iter);
	m_trianglecount = read<uint32_t>(iter);
	m_vertexcount = read<uint32_t>(iter);
	uint8_t* vertOffset = const_cast<uint8_t*>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (int i = 0; i < m_vertexcount; ++i)
	{
		glm::vec2 vert = read<glm::vec2>(vertOffset);
		m_vertices.push_back(vert);
	}
	uint8_t* triOffset = const_cast<uint8_t*>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (int i = 0; i < m_trianglecount; ++i)
	{
		glm::u16vec3 tri = read<glm::u16vec3>(triOffset);
		m_triangles.push_back(tri);
	}
	m_recordcount = read<uint32_t>(iter);
	uint8_t* recordOffset = const_cast<uint8_t*>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (int i = 0; i < m_recordcount; ++i)
	{
		Record r = read<Record>(recordOffset);
		m_records.push_back(r);
	}
	m_actioncount = read<uint32_t>(iter);
	uint8_t* actionOffset = const_cast<uint8_t*>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (int i = 0; i < m_actioncount; ++i)
	{
		Action a = read<Action>(actionOffset);
		m_actions.push_back(a);
	}
	m_unknown2 = read<uint32_t>(iter);
}

void Button::Update(const Transformation& t, std::shared_ptr<DisplayObject> instance)
{
	int a = 0;
}
