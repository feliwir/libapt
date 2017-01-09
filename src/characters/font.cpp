#include "font.hpp"
#include <libapt/apt.hpp>
#include "../util.hpp"
#include <iostream>
using namespace libapt;

void Font::Parse(uint8_t*& iter)
{
	m_name = reinterpret_cast<const char*>(m_owner->GetBase() + read<uint32_t>(iter));
	m_glyphcount = read<uint32_t>(iter);
	m_glyphs = const_cast<uint8_t*>(m_owner->GetBase() + read<uint32_t>(iter));
}

void Font::Update(const Transformation& t, std::shared_ptr<DisplayObject> dObj)
{
	std::cout << "WARNING: Font should not be able to be on displaylist" << std::endl;
}