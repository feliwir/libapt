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

Container::Container()
{
}

void Container::HandleAction(std::shared_ptr<FrameItem> fi)
{
	auto action = std::dynamic_pointer_cast<Action>(fi);
	m_sc.Execute(action->GetBytecode(),shared_from_this());
}

void  Container::HandleInitAction(std::shared_ptr<FrameItem> fi)
{
	auto action = std::dynamic_pointer_cast<InitAction>(fi);
	m_sc.Execute(action->GetBytecode(), shared_from_this());
}

void Container::HandlePlaceObject(std::shared_ptr<FrameItem> fi)
{
	auto po = std::dynamic_pointer_cast<PlaceObject>(fi);
	if (po->HasMove())
	{
		if(po->HasMatrix())
			m_dl.Move(po->GetDepth(), po->GetTranslate(), po->GetRotScale());
	}
	else
	{
		auto ch = m_owner->GetCharacter(po->GetCharacter());
		if (ch != nullptr)
		{
			m_dl.Insert(po->GetDepth(), ch, po->GetTranslate(),
				po->GetRotScale(), po->GetName());
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

void Container::Update(const Transformation& t)
{
	if (m_currentFrame < m_framecount)
	{
		Frame& cFrame = m_frames[m_currentFrame];

		for (const auto& fi : cFrame.GetFrameitems())
		{
			if (fi == nullptr)
				continue;

			switch (fi->GetType())
			{
			case FrameItem::ACTION:
				HandleAction(fi);
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
				HandleInitAction(fi);
				break;
			}
		}
		m_currentFrame++;
	}
	
	m_dl.Render(t);
}

