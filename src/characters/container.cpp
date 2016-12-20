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
	auto characters = m_owner->GetCharacters();
	auto it = m_displayObjects.find(po->GetDepth());

	DisplayObject object;
	//add the displayobject
	if (it == m_displayObjects.end())
	{
		object.rotscale = po->GetRotScale();
		object.translate = po->GetTranslate();
		auto c = characters[po->GetCharacter()];
		if (c == nullptr)
		{
			std::cout << "Can't place null character" << std::endl;
			return;
		}
		object.ch = c->MakeInstance();
		m_displayObjects[po->GetDepth()] = object;
	}
	//object already exists
	else
	{
		if (po->GetCharacter() != -1)
		{
			std::cout << "Can't place another character at depth " << po->GetDepth() << std::endl;
			return;
		}

		it->second.rotscale = po->GetRotScale();
		it->second.translate = po->GetTranslate();
	}
	
}

void Container::HandleRemoveObject(std::shared_ptr<FrameItem> fi)
{
	auto ro = std::dynamic_pointer_cast<RemoveObject>(fi);
	m_displayObjects.erase(ro->GetDepth());
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
	
	for (auto& obj : m_displayObjects)
	{
		Transformation t2;
		t2.rotscale = t.rotscale*obj.second.rotscale;
		t2.translate = t.translate + obj.second.translate;
		
		obj.second.ch->Update(t2);
	}
	int a = 0;
}

