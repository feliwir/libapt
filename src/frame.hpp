#pragma once
#include "frameitems/frameitem.hpp"
#include <stdint.h>
#include <memory>
#include <vector>

namespace libapt
{
	class Frame
	{
	public:
		void Parse(const uint8_t* offset,const uint8_t* base);

		//bytes to skip for each frame
		static const uint32_t FRAMESIZE = 8;
		inline const std::vector<std::shared_ptr<FrameItem>>& GetFrameitems() { return m_items; }
	private:
		uint32_t m_itemCount;
		std::vector<std::shared_ptr<FrameItem>> m_items;
	};
}