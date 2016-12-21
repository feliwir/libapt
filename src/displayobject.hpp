#pragma once
#include "characters/character.hpp"
#include <memory>

namespace libapt
{
	class DisplayObject
	{
	public:
		void Create(std::shared_ptr<Character> ch, const glm::vec2& translate,
			const glm::mat2& rotscale, const std::string& name);
		void Render(const Transformation& t);

		inline void SetTranslate(const glm::vec2& t)
		{
			m_translate = t;
		}
		inline void SetRotscale(const glm::mat2& rotscale)
		{
			m_rotscale = rotscale;
		}
	private:
		std::shared_ptr<Character> m_character;
		glm::u8vec4 m_color;
		glm::vec2 m_translate;
		glm::mat2 m_rotscale;
		std::string m_name;
	};
}