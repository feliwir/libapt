#pragma once
#include "character.hpp"
#include <glm/glm.hpp>

namespace libapt
{
	class Edittext : public Character
	{
	public:
		virtual void Parse(uint8_t *&iter) override;
		virtual void Update(const Transformation& t, std::shared_ptr<DisplayObject> instance) override;
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