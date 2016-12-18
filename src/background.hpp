#pragma once
#include "frameitem.hpp"

namespace libapt
{
	class Background : public FrameItem
	{
	public:
		virtual void Parse(uint8_t *offset, const uint8_t *base);
		inline uint32_t GetColor() { return m_color; }
	private:
		uint32_t m_color;
	};
}