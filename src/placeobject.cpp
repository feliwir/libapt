#include "placeobject.hpp"
#include "util.hpp"
#include <iostream>
using namespace libapt;

void PlaceObject::Parse(uint8_t *offset, const uint8_t *base)
{
	m_flags = read<uint32_t>(offset);
	m_depth = read<int32_t>(offset);
	m_character = read<int32_t>(offset);
	m_rotscale = read<glm::f32mat2>(offset);
	m_translate = read<glm::f32vec2>(offset);
	m_color = read<glm::u8vec4>(offset);
	m_unknown = read<uint32_t>(offset);
	m_ratio = read<glm::f32>(offset);
	uint32_t nameOffset = read<uint32_t>(offset);
	if(nameOffset)
		m_name = readString(base+nameOffset);
	m_clipdepth = read<int32_t>(offset);
	uint32_t poaOffset = read<uint32_t>(offset);
	if (poaOffset)
	{
		uint8_t* poa_offset = const_cast<uint8_t*>(base) + poaOffset;
		auto fi = FrameItem::Create(poa_offset, base);
		if (fi->GetType() != ACTION)
		{
			std::cout << "WTF mate" << std::endl;
		}

		//m_action = std::dynamic_pointer_cast<Action>(fi);
	}
}