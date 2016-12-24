#include "displayobject.hpp"
#include "characters/container.hpp"
using namespace libapt;

void DisplayObject::Create(std::shared_ptr<Character> ch, const glm::vec2 & translate,
	const glm::mat2& rotscale, const std::string & name, std::shared_ptr<Container> parent)
{
	m_character = ch->MakeInstance();
	m_rotscale = rotscale;
	m_translate = translate;
	m_name = name;
	m_parent = parent;
}

void DisplayObject::Render(const Transformation& t)
{
	Transformation cTransform;
	cTransform.rotscale = t.rotscale*m_rotscale;
	cTransform.translate = t.translate + m_translate;
	m_character->Update(cTransform,*this);
}

void DisplayObject::OnPropertyChanged(const std::string& property, as::Value v)
{

}