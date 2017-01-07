#include "displayobject.hpp"
#include "characters/container.hpp"
using namespace libapt;

DisplayObject::DisplayObject() : m_character(nullptr), m_clipDepth(0), m_color(1.0),
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
	m_mask = std::make_shared<ClipMask>(ch->GetOwner()->GetWidth(), ch->GetOwner()->GetHeight());
}

bool IsProblem(const std::string& bla)
{
	return (bla == "NavFrameFive");
}

void DisplayObject::Render(const Transformation& t)
{
	if (IsClippingLayer())
	{
		if (m_character->GetOwner()->HasResized())
		{
			m_mask->ResizeFb(m_character->GetOwner()->GetWidth(),
				m_character->GetOwner()->GetHeight());
		}
		m_mask->BindFb();
		m_mask->Clear();
	}

	Transformation cTransform;
	cTransform.rotscale = t.rotscale*m_rotscale;
	cTransform.translate = t.translate + m_translate;
	cTransform.color = t.color * m_color;
	cTransform.mask = t.mask;
	m_character->Update(cTransform,*this);

	if (IsClippingLayer())
		m_mask->UnbindFb();
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