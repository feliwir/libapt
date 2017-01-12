#pragma once
#include "frameitem.hpp"
#include <string>

namespace libapt
{
	class Framelabel : public FrameItem
	{
	public:
		virtual void Parse(uint8_t *offset, const uint8_t *base) override;

		inline const std::string& GetLabel()
		{
			return m_label;
		}

		inline uint32_t GetFrame()
		{
			return m_frameid;
		}
	private:
		std::string m_label;
		uint32_t m_flags;
		uint32_t m_frameid;
	};
}