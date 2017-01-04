#include "container.hpp"
#include <libapt/apt.hpp>
#include "../frameitems/action.hpp"
#include "../frameitems/background.hpp"
#include "../frameitems/initaction.hpp"
#include "../frameitems/placeobject.hpp"
#include "../frameitems/removeobject.hpp"
#include "../graphics/flextGL.h"
#include <iostream>
using namespace libapt;
as::Engine Container::s_engine;

Container::Container() : m_playing(true)
{
}

void Container::HandleAction(std::shared_ptr<FrameItem> fi,DisplayObject& dispO)
{
	auto action = std::dynamic_pointer_cast<Action>(fi);
	s_engine.Execute(dispO,action->GetBytecode(),m_owner);
}

void  Container::HandleInitAction(std::shared_ptr<FrameItem> fi, DisplayObject& dispO)
{
	auto action = std::dynamic_pointer_cast<InitAction>(fi);
	s_engine.Execute(dispO, action->GetBytecode(), m_owner);
}

void Container::HandlePlaceObject(std::shared_ptr<FrameItem> fi)
{
	auto po = std::dynamic_pointer_cast<PlaceObject>(fi);
	if (po->HasMove())
	{
		if (po->HasColortransform())
			int a = 0;

		if(po->HasMatrix())
			m_dl.Move(po->GetDepth(), po->GetTranslate(), po->GetRotScale());

		if (po->HasClipActions())
		{
			auto action = po->GetClipActions();
		}
	}
	else
	{
		auto ch = m_owner->GetCharacter(po->GetCharacter());
		if (ch != nullptr)
		{
			m_dl.Insert(po->GetDepth(), ch, po->GetTranslate(),
				po->GetRotScale(), po->GetName(),
				std::dynamic_pointer_cast<Container>(shared_from_this()));

			if (po->HasClipActions())
			{
				auto action = po->GetClipActions();
			}
			if (po->HasColortransform())
			{
				int a = 0;
			}
		}	
	}	
}

void Container::HandleRemoveObject(std::shared_ptr<FrameItem> fi)
{
	auto ro = std::dynamic_pointer_cast<RemoveObject>(fi);
	m_dl.Erase(ro->GetDepth());
}

void Container::HandleBackground(std::shared_ptr<FrameItem> fi)
{
	auto bg = std::dynamic_pointer_cast<Background>(fi);
	m_bgColor = glm::vec4(bg->GetColor());
	m_bgColor /= 255.0;
	glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
}

void Container::Update(const Transformation& t, DisplayObject& dispO)
{
	if (m_currentFrame < m_framecount && m_playing)
	{
		Frame& cFrame = m_frames[m_currentFrame];

		for (const auto& fi : cFrame.GetFrameitems())
		{
			if (fi == nullptr)
				continue;

			switch (fi->GetType())
			{
			case FrameItem::ACTION:
				HandleAction(fi,dispO);
				break;
			case FrameItem::PLACEOBJECT:
				HandlePlaceObject(fi);
				break;
			case FrameItem::REMOVEOBJECT:
				HandleRemoveObject(fi);
				break;
			case FrameItem::BACKGROUNDCOLOR:
				HandleBackground(fi);
				break;
			case FrameItem::INITACTION:
				HandleInitAction(fi,dispO);
				break;
			}
		}

		m_currentFrame++;

		if (m_currentFrame == m_frames.size())
		{
			m_currentFrame = 0;
		}
	}

	m_dl.Render(t);
}

