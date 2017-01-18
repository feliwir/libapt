#include "button.hpp"
#include "../util.hpp"
#include "../displayobject.hpp"
#include "../actionscript/engine.hpp"
#include "../graphics/debug.hpp"
#include <libapt/apt.hpp>
#include <libapt/manager.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>
using namespace libapt;

Button::Button() : m_state(IDLE)
{
}

void Button::Parse(uint8_t *&iter)
{
	//reserved flags
	m_unknown = read<uint32_t>(iter);
	m_bounds = read<glm::vec4>(iter);
	m_trianglecount = read<uint32_t>(iter);
	m_vertexcount = read<uint32_t>(iter);
	uint8_t *vertOffset = const_cast<uint8_t *>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (int i = 0; i < m_vertexcount; ++i)
	{
		glm::vec2 vert = read<glm::vec2>(vertOffset);
		m_vertices.push_back(vert);
	}
	uint8_t *triOffset = const_cast<uint8_t *>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (int i = 0; i < m_trianglecount; ++i)
	{
		glm::u16vec3 tri = read<glm::u16vec3>(triOffset);
		m_triangles.push_back(tri);
	}
	m_recordcount = read<uint32_t>(iter);
	uint8_t *recordOffset = const_cast<uint8_t *>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (int i = 0; i < m_recordcount; ++i)
	{
		Record r = read<Record>(recordOffset);
		m_records.push_back(r);
	}
	m_actioncount = read<uint32_t>(iter);
	uint8_t *actionOffset = const_cast<uint8_t *>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (int i = 0; i < m_actioncount; ++i)
	{
		Action a;
		a.flags = read<ActionFlags>(actionOffset);
		a.bytecode = m_owner->GetBase() + read<uint32_t>(actionOffset);
		m_actions.push_back(a);
	}
	m_unknown2 = read<uint32_t>(iter);
	CreateDebugBuffer();
}

void Button::CreateDebugBuffer()
{
	std::vector<glm::vec2> verts;
	for (auto &t : m_triangles)
	{
		verts.push_back(m_vertices[t.x]);
		verts.push_back(m_vertices[t.y]);
		verts.push_back(m_vertices[t.z]);
	}
	m_buffer.Append(verts);
	m_buffer.Finalize();
	m_color = glm::vec4(1.0, 0.0, 0.0, 0.3);
}

void Button::Update(const Transformation &t, std::shared_ptr<DisplayObject> instance)
{
	auto mngr = m_owner->GetManager();
	if (mngr->GetDebug())
		Debug::Draw(m_buffer, t, m_color);

	double x, y;
	mngr->GetMousePosition(x, y);
	if (x == 0 && y == 0)
		return;
	State last = m_state;
	m_state = IDLE;
	glm::vec2 p(x, y);

	if (instance->GetName().size() > 0 && instance->GetName() != "bttn")
		int a = 0;

	double xratio = static_cast<double>(mngr->GetWidth()) / m_owner->GetWidth();
	double yratio = static_cast<double>(mngr->GetHeight()) / m_owner->GetHeight();

	for (const auto &tri : m_triangles)
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
		const double denom = ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));

		const double a = ((v2.y - v3.y) * (p.x - v3.x) + (v3.x - v2.x) * (p.y - v3.y)) / denom;

		const double b = ((v3.y - v1.y) * (p.x - v3.x) + (v1.x - v3.x) * (p.y - v3.y)) / denom;

		const double c = 1 - a - b;

		if ((0 <= a) && (0 <= b) && (0 <= c) &&
			(a <= 1) && (b <= 1) && (c <= 1))
		{
			//p is inside the triangle
			if (instance->GetName().size() > 0)
				std::cout << "Mouse over: " << instance->GetName() << std::endl;

			m_state = OVERUP;
			break;
		}
	}

	for (auto &a : m_actions)
	{
		ActionFlags flag = a.flags;
		if (last == IDLE && m_state == OVERUP)
		{
			if (flag.IdleToOverUp)
			{
				as::Engine::s_engine.Execute(instance->GetParent(), a.bytecode, m_owner);
			}
		}
		if (last == OVERUP && m_state == IDLE)
		{
			if (flag.OverUpToIdle)
			{
				as::Engine::s_engine.Execute(instance->GetParent(), a.bytecode, m_owner);
			}
		}
	}

	switch (m_state)
	{
	case IDLE:
		if(mngr->GetButtonDown()==reinterpret_cast<uintptr_t>(instance.get()))
			mngr->SetButtonDown(0);
		m_color = glm::vec4(1.0, 0.0, 0.0, 0.3);
		break;
	case OVERUP:
		mngr->SetButtonDown(reinterpret_cast<uintptr_t>(instance.get()));
		m_color = glm::vec4(0.0, 1.0, 0.0, 0.3);
		break;
	}
}
