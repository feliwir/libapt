#include "displayobject.hpp"
#include "characters/button.hpp"
#include "characters/container.hpp"
#include <libapt/manager.hpp>
#include <iostream>
using namespace libapt;

DisplayObject::DisplayObject() : m_character(nullptr), m_clipDepth(0), m_color(1.0),
m_isClipLayer(false), m_visible(true)
{
}

void DisplayObject::Create(std::shared_ptr<Character> ch, const glm::vec2 & translate,
	const glm::mat2& rotscale, const std::string & name, std::shared_ptr<DisplayObject> parent)
{
	auto this_ptr = std::dynamic_pointer_cast<DisplayObject>(shared_from_this());
	m_character = ch->MakeInstance();
	m_character->Prepare(this_ptr);
	m_rotscale = rotscale;
	m_translate = translate;
	m_name = name;
	m_parent = parent;
	m_ps = PLAYING;
	m_isClipLayer = false;

}

void DisplayObject::CreateClipLayer(std::shared_ptr<Character> ch, const glm::vec2 & translate, 
	const glm::mat2 & rotscale, const std::string & name, 
	std::shared_ptr<DisplayObject> parent, uint32_t clipdepth)
{
	auto this_ptr = std::dynamic_pointer_cast<DisplayObject>(shared_from_this());
	m_character = ch->MakeInstance();
	m_character->Prepare(this_ptr);
	m_rotscale = rotscale;
	m_translate = translate;
	m_name = name;
	m_parent = parent;
	m_ps = PLAYING;
	m_isClipLayer = true;
	m_clipDepth = clipdepth;
	m_mask = std::make_shared<ClipMask>(
		m_character->GetOwner()->GetManager()->GetWidth(),
		m_character->GetOwner()->GetManager()->GetHeight());

}

void DisplayObject::Render(const Transformation& t)
{
	if (IsClippingLayer())
	{
		if (m_character->GetOwner()->HasResized())
		{
			m_mask->ResizeFb(
				m_character->GetOwner()->GetManager()->GetWidth(),
				m_character->GetOwner()->GetManager()->GetHeight());
		}
		m_mask->BindFb();
		m_mask->Clear();
	}

	Transformation cTransform;
	cTransform.rotscale = t.rotscale*m_rotscale;
	cTransform.translate = t.translate + m_translate;
	cTransform.color = t.color * m_color;
	cTransform.mask = t.mask;
	cTransform.visible = (t.visible) ? m_visible : false;
	cTransform.playing = (t.playing) ? (m_ps==as::Object::PLAYING) : false;
	auto this_ptr = std::dynamic_pointer_cast<DisplayObject>(shared_from_this());
	m_character->Update(cTransform, this_ptr);

	if (IsClippingLayer())
		m_mask->UnbindFb();

	//only the case for the root object
	if(m_focus!=nullptr)
	{
		auto button = std::dynamic_pointer_cast<Button>(m_focus->GetCharacter());
		button->OnFocus(m_focus);
	}
}

void DisplayObject::OnPropertyChanged(const std::string& property)
{
	//check if any builtin property was changed
	if (property == "_visible")
	{
		m_visible = m_properties[property].ToBoolean();
	}
	if (property == "_alpha")
	{
		m_color.a = m_properties[property].ToInteger() / 255.0;
	}
}

