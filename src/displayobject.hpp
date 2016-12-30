#pragma once
#include "characters/character.hpp"
#include "actionscript/object.hpp"
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace libapt
{
	class Container;

	class DisplayObject : public as::Object
	{
	public:
		void Create(std::shared_ptr<Character> ch, const glm::vec2& translate,
			const glm::mat2& rotscale, const std::string& name,
			std::shared_ptr<Container> parent);
		void Render(const Transformation& t);

		inline void SetTranslate(const glm::vec2& t)
		{
			m_translate = t;
		}

		inline void SetRotscale(const glm::mat2& rotscale)
		{
			m_rotscale = rotscale;
		}

		inline void SetCharacter(const std::shared_ptr<Character> ch)
		{
			m_character = ch;
		}

		inline const std::shared_ptr<Character> GetCharacter()
		{
			return m_character;
		}

		virtual void OnPropertyChanged(const std::string& property);
		virtual void OnPlayStateChanged();
	private:
		std::shared_ptr<Character> m_character;
		std::shared_ptr<Container> m_parent;
		glm::u8vec4 m_color;
		glm::vec2 m_translate;
		glm::mat2 m_rotscale;
		std::string m_name;
	};
}