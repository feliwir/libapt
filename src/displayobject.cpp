#include "displayobject.hpp"
#include "characters/container.hpp"
using namespace libapt;

DisplayObject::DisplayObject() : m_character(0), m_clipDepth(0),
m_isClipLayer(false)
{
}

void DisplayObject::Create(std::shared_ptr<Character> ch, const glm::vec2 & translate,
	const glm::mat2& rotscale, const std::string & name, std::shared_ptr<Container> parent)
{
	m_character = ch->MakeInstance();
	m_rotscale = rotscale;
	m_translate = translate;
	m_name = name;
	m_parent = parent;
	m_ps = PLAYING;
	m_isClipLayer = false;
}

void DisplayObject::CreateClipLayer(std::shared_ptr<Character> ch, const glm::vec2 & translate, 
	const glm::mat2 & rotscale, const std::string & name, 
	std::shared_ptr<Container> parent, uint32_t clipdepth)
{
	m_character = ch->MakeInstance();
	m_rotscale = rotscale;
	m_translate = translate;
	m_name = name;
	m_parent = parent;
	m_ps = PLAYING;
	m_isClipLayer = true;
	m_clipDepth = clipdepth;
}

void DisplayObject::Render(const Transformation& t)
{
	Transformation cTransform;
	cTransform.rotscale = t.rotscale*m_rotscale;
	cTransform.translate = t.translate + m_translate;
	m_character->Update(cTransform,*this);
}


void DisplayObject::OnPropertyChanged(const std::string& property)
{

}

void DisplayObject::OnPlayStateChanged()
{
	auto c = std::dynamic_pointer_cast<Container>(m_character);
	c->SetPlaying((m_ps == PLAYING));
}

void DisplayObject::OnFrameChanged()
{
	auto c = std::dynamic_pointer_cast<Container>(m_character);
	c->SetFrame(m_cf);
}