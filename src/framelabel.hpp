#pragma once
#include "frameitem.hpp"

namespace libapt
{
	class Framelabel : public FrameItem
	{
	public:
		virtual void Parse(uint8_t *offset, const uint8_t *base);
	private:
		std::string m_label;
		uint32_t m_flags;
		uint32_t m_frameid;
	};
}