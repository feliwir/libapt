#include "displayobject.hpp"
using namespace libapt;

void DisplayObject::Create(std::shared_ptr<Character> ch, const glm::vec2 & translate,
	const glm::mat2& rotscale, const std::string & name)
{
	m_character = ch->MakeInstance();
	m_rotscale = rotscale;
	m_translate = translate;
	m_name = name;

}

void DisplayObject::Render(const Transformation& t)
{
	Transformation cTransform;
	cTransform.rotscale = t.rotscale*m_rotscale;
	cTransform.translate = t.translate + m_translate;
	m_character->Update(cTransform);
}