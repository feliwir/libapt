#pragma once
#include "character.hpp"
#include <glm/glm.hpp>

namespace libapt
{
	class Edittext : public Character
	{
	public:
		virtual void Parse(uint8_t *&iter);
		//Update all placed objects
		virtual void Update(const Transformation& t);
	private:
		glm::vec4 m_bounds;
		uint32_t m_font;
		uint32_t m_alignment;
		glm::u8vec4 m_color;
		float m_fontheight;
		uint32_t m_readonly;
		uint32_t m_multiline;
		uint32_t m_wordwrap;
		const char* m_text;
		const char* m_variable;
	};
}