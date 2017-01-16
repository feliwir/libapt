#include "button.hpp"
#include "../util.hpp"
#include "../displayobject.hpp"
#include "../actionscript/engine.hpp"
#include <libapt/apt.hpp>
#include <libapt/manager.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>
using namespace libapt;

Button::Button() : m_state(NONE)
{

}

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
		a.actiondata += reinterpret_cast<uintptr_t>(m_owner->GetBase());
		m_actions.push_back(a);
	}
	m_unknown2 = read<uint32_t>(iter);
}

void Button::Update(const Transformation& t, std::shared_ptr<DisplayObject> instance)
{
	if (m_owner->GetFrameevent())
		return;

	auto mngr = m_owner->GetManager();
	double x, y;
	mngr->GetMousePosition(x, y);
	State last = m_state;
	m_state = NONE;
	glm::vec2 p(x, y);

	if (x == 0 && y == 0)
		return;

	double xratio = static_cast<double>(m_owner->GetManager()->GetWidth())
		/ m_owner->GetWidth();
	double yratio = static_cast<double>(m_owner->GetManager()->GetHeight())
		/ m_owner->GetHeight();

	for (const auto& tri : m_triangles)
	{
		auto v1 = m_vertices[tri.x];
		auto v2 = m_vertices[tri.y];
		auto v3 = m_vertices[tri.z];
		v1 = t.rotscale * v1;
		v2 = t.rotscale * v2;
		v3 = t.rotscale * v3;
		v1 += t.translate;
		v2 += t.translate;
		v3 += t.translate;

		v1.x *= xratio;
		v2.x *= xratio;
		v3.x *= xratio;

		v1.y *= yratio;
		v2.y *= yratio;
		v3.y *= yratio;

		//barycentric coordinates
		const double denom = ((v2.y - v3.y)*(v1.x - v3.x) + (v3.x - v2.x)*(v1.y - v3.y));

		const double a = ((v2.y - v3.y)*(p.x - v3.x) + (v3.x - v2.x)*(p.y - v3.y)) / denom;
			
		const double b = ((v3.y - v1.y)*(p.x - v3.x) + (v1.x - v3.x)*(p.y - v3.y)) / denom;

		const double c = 1 - a - b;

		if ((0 <= a) && (0 <= b) && (0 <= c) &&
			(a <= 1) && (b <= 1) && (c <= 1))
		{
			//p is inside the triangle
			if(instance->GetName().size()>0)
				std::cout << "Mouse over: " << instance->GetName() << std::endl;

			m_state = HOVER;
			m_owner->HadFrameevent();
			break;
		}
	}

	for (auto& a : m_actions)
	{
		ActionFlags flag = a.flags;
		if (last == NONE && m_state == HOVER)
		{
			if (flag.IdleToOverDown)
			{
				as::Engine::s_engine.Execute(instance->GetParent(), a.actiondata, m_owner);
			}
		}
		if (last == HOVER && m_state == NONE)
		{
			if (flag.OutDownToIdle)
			{
				as::Engine::s_engine.Execute(instance->GetParent(), a.actiondata, m_owner);
			}
		}
	}

}
