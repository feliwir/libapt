#include "placeobject.hpp"
#include "../util.hpp"
#include <iostream>
using namespace libapt;

PlaceObject::PlaceObject() :
m_character(-1),
m_rotscale(glm::mat2(1.0)),
m_translate(0.0),
m_color(255),
m_ratio(0),
m_clipdepth(-1)
{}

void PlaceObject::Parse(uint8_t* iter, const uint8_t *base)
{
	m_flags = read<Flags>(iter);
	m_depth = read<int32_t>(iter);

	if(HasCharacter())
		m_character = read<int32_t>(iter);
	else
		skip<int32_t>(iter);
	
	if (HasMatrix())
		m_rotscale = read<glm::f32mat2>(iter);
	else
		skip<glm::f32mat2>(iter);

	if (HasMatrix())
		m_translate = read<glm::f32vec2>(iter);
	else
		skip<glm::f32vec2>(iter);

	if (HasColortransform())
		m_color = read<glm::u8vec4>(iter);
	else
		skip<glm::u8vec4>(iter);

	m_unknown = read<uint32_t>(iter);

	if (HasRatio())
		m_ratio = read<glm::f32>(iter);
	else
		skip<glm::f32>(iter);

	uint32_t nameOffset = 0;
	if (HasName())
		nameOffset = read<uint32_t>(iter);
	else
		skip<uint32_t>(iter);

	if(nameOffset)
		m_name = readString(base+nameOffset);

	m_clipdepth = read<int32_t>(iter);
	
	
	if (m_flags.PlaceFlagHasClipActions)
	{
		ParseClipActions(iter, base);
	}
	else
		skip<uint64_t>(iter);
}

void PlaceObject::ParseClipActions(uint8_t*& iter, const uint8_t *base)
{
	uint8_t* poaOffset = const_cast<uint8_t*>(base) + read<uint32_t>(iter);

	m_clipactions.count = read<uint32_t>(iter);
	uint8_t* actionsOffset = const_cast<uint8_t*>(base) + read<uint32_t>(iter);
	for (uint32_t i = 0; i < m_clipactions.count; ++i)
	{
		ClipAction ca;
		skip<uint32_t>(actionsOffset);
		ca.flags = read<ClipEventFlags>(actionsOffset);
		ca.bytes = const_cast<uint8_t*>(base) + read<uint32_t>(actionsOffset);
	}
}