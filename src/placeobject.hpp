#pragma once
#include "frameitem.hpp"
#include "action.hpp"
#include <glm/glm.hpp>

namespace libapt
{
	class PlaceObject : public FrameItem
	{
	public:
		virtual void Parse(uint8_t *offset, const uint8_t *base);

		inline int32_t GetDepth() { return m_depth; }
		inline uint32_t GetCharacter() { return m_character; }
		inline glm::f32mat2 GetRotScale() { return m_rotscale; }
		inline glm::f32vec2 GetTranslate() { return m_translate; }
	private:
		uint32_t m_flags;
		int32_t m_depth;
		int32_t m_character;
		glm::f32mat2 m_rotscale;
		glm::f32vec2 m_translate;
		glm::u8vec4 m_color;
		uint32_t m_unknown;
		glm::f32 m_ratio;
		std::string m_name;
		int32_t m_clipdepth;
		Action m_action;
	};
}