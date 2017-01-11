#pragma once
#include "frameitem.hpp"

namespace libapt
{
	class RemoveObject : public FrameItem
	{
	public:
		virtual void Parse(uint8_t *offset, const uint8_t *base) override;
		
		inline int32_t GetDepth() { return m_depth; }
	private:
		int32_t m_depth;
	};
}