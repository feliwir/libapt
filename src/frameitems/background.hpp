#pragma once
#include "frameitem.hpp"
#include <glm/glm.hpp>

namespace libapt
{
	class Background : public FrameItem
	{
	public:
		virtual void Parse(uint8_t *offset, const uint8_t *base);
		inline glm::u8vec4 GetColor() { return m_color; }
	private:
		glm::u8vec4 m_color;
	};
}