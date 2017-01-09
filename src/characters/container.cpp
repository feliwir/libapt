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

void Container::HandleAction(std::shared_ptr<FrameItem> fi, std::shared_ptr<DisplayObject> instance)
{
	auto action = std::dynamic_pointer_cast<Action>(fi);
	s_engine.Execute(instance,action->GetBytecode(),m_owner);
}

void  Container::HandleInitAction(std::shared_ptr<FrameItem> fi, std::shared_ptr<DisplayObject> instance)
{
	auto action = std::dynamic_pointer_cast<InitAction>(fi);
	s_engine.Execute(instance, action->GetBytecode(), m_owner);
}

void Container::HandlePlaceObject(std::shared_ptr<FrameItem> fi, std::shared_ptr<DisplayObject> instance)
{
	auto po = std::dynamic_pointer_cast<PlaceObject>(fi);

	//Move character
	if (po->HasMove() && !po->HasCharacter())
	{	
		if(po->HasMatrix())
			m_dl.Move(po->GetDepth(), po->GetTranslate(), po->GetRotScale());

	} 
	//New character
	else if(!po->HasMove() && po->HasCharacter())
	{
		auto ch = m_owner->GetCharacter(po->GetCharacter());
		if (ch != nullptr)
		{

			if (po->HasClipDepth())
			{
				m_dl.AddClipLayer(po->GetDepth(),po->GetClipDepth(),ch,po->GetTranslate(),
					po->GetRotScale(), po->GetName(), instance);
			}
			else
			{
				m_dl.Insert(po->GetDepth(), ch, po->GetTranslate(),
					po->GetRotScale(), po->GetName(), instance);
			}

			if (po->HasClipActions())
			{
				auto action = po->GetClipActions();
			}
		}
		else
		{
			std::cout << "Missing character" << std::endl;
		}
	}
	//Erase and place new
	else if (po->HasMove() && po->HasCharacter())
	{
		m_dl.Erase(po->GetDepth());
		auto ch = m_owner->GetCharacter(po->GetCharacter());
		if (ch != nullptr)
		{
			m_dl.Insert(po->GetDepth(), ch, po->GetTranslate(),
				po->GetRotScale(), po->GetName(), instance);

			if (po->HasClipActions())
			{
				auto action = po->GetClipActions();
			}
		}
	}
	else
	{
		int a = 0;
	}
	if (po->HasColortransform())
	{
		m_dl.Colortransform(po->GetDepth(), po->GetColor());
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

void Container::Update(const Transformation& t, std::shared_ptr<DisplayObject> instance)
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
				HandleAction(fi, instance);
				break;
			case FrameItem::PLACEOBJECT:
				HandlePlaceObject(fi,instance);
				break;
			case FrameItem::REMOVEOBJECT:
				HandleRemoveObject(fi);
				break;
			case FrameItem::BACKGROUNDCOLOR:
				HandleBackground(fi);
				break;
			case FrameItem::INITACTION:
				HandleInitAction(fi, instance);
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

