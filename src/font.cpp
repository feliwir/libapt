#include "font.hpp"
#include "util.hpp"
#include <libapt/apt.hpp>
using namespace libapt;

void Font::Parse(uint8_t*& iter)
{
	m_name = reinterpret_cast<const char*>(m_owner->GetBase() + read<uint32_t>(iter));
	m_glyphcount = read<uint32_t>(iter);
	m_glyphs = const_cast<uint8_t*>(m_owner->GetBase() + read<uint32_t>(iter));
}

void Font::Update(Transformation t)
{

}