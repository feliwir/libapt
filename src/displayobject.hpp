#pragma once
#include "characters/character.hpp"
#include "actionscript/object.hpp"
#include "graphics/clipmask.hpp"
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace libapt
{
	class DisplayObject : public as::Object
	{
	public:
		DisplayObject();

		void Create(std::shared_ptr<Character> ch, const glm::vec2& translate,
			const glm::mat2& rotscale, const std::string& name,
			std::shared_ptr<DisplayObject> parent);

		void Delete();

		void CreateClipLayer(std::shared_ptr<Character> ch, const glm::vec2& translate,
			const glm::mat2& rotscale, const std::string& name,
			std::shared_ptr<DisplayObject> parent, uint32_t clipdepth);

		void Render(const Transformation& t);

		inline void SetColor(const glm::u8vec4& color)
		{
			m_color = glm::vec4(color.r / 255.0, color.g / 255.0,
				color.b / 255.0, color.a / 255.0);
		}

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

		inline bool IsClippingLayer()
		{
			return m_isClipLayer;
		}

		inline std::string GetName()
		{
			return m_name;
		}

		inline std::shared_ptr<ClipMask> GetClippingLayer()
		{
			return m_mask;
		}

		inline uint32_t GetClipDepth()
		{
			return m_clipDepth;
		}

		virtual void OnPropertyChanged(const std::string& property);
		virtual void OnPlayStateChanged();
		virtual void OnFrameChanged();
	private:	
		std::shared_ptr<Character> m_character;
		std::shared_ptr<DisplayObject> m_parent;
		glm::vec4 m_color;
		glm::vec2 m_translate;
		glm::mat2 m_rotscale;
		std::string m_name;
		bool m_isClipLayer;
		uint32_t m_clipDepth;
		std::shared_ptr<ClipMask> m_mask;
	};
}