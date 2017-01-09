#include "edittext.hpp"
#include <libapt/apt.hpp>
#include "../util.hpp"

using namespace libapt;

void Edittext::Parse(uint8_t *&iter)
{
	m_bounds = read<glm::vec4>(iter);
	m_font = read<uint32_t>(iter);
	m_alignment = read<uint32_t>(iter);
	m_color = read<glm::u8vec4>(iter);
	m_fontheight = read<float>(iter);
	m_readonly = read<uint32_t>(iter);
	m_multiline = read<uint32_t>(iter);
	m_wordwrap = read<uint32_t>(iter);
	m_text = reinterpret_cast<const char*>(m_owner->GetBase() + read<uint32_t>(iter));
	m_variable = reinterpret_cast<const char*>(m_owner->GetBase() + read<uint32_t>(iter));
}

void Edittext::Update(const Transformation& t, std::shared_ptr<DisplayObject> dObj)
{

}