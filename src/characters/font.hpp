#pragma once
#include "character.hpp"
#include <libapt/texture.hpp>
#include <vector>

namespace libapt
{
	class Font : public Character
	{
	public:
		virtual void Parse(uint8_t*& iter);
		virtual void Update(const Transformation& t, DisplayObject& dObj);
	private:
		const char* m_name;
		uint32_t m_glyphcount;
		uint8_t* m_glyphs;
	};
}