#pragma once
#include "frameitem.hpp"

namespace libapt
{
    class InitAction : public FrameItem
    {
    public:
		virtual void Parse(uint8_t *offset, const uint8_t *base);
		inline const uint8_t* GetBytecode()
		{
			return m_bytes;
		}
    private:
        uint32_t m_sprite;
		uint8_t* m_bytes;
    };
}