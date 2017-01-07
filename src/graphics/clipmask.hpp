#pragma once
#include <stdint.h>

namespace libapt
{
    class ClipMask
    {
	public:
		ClipMask(uint32_t width,uint32_t height);
		~ClipMask();

		void Clear();
		void BindFb();
		void UnbindFb();

		void ResizeFb(uint32_t width, uint32_t height);

		void BindMask();
	private:
		uint32_t	m_fbId;
		uint32_t	m_texId;
		uint32_t	m_dbId;
		uint32_t	m_width;
		uint32_t	m_height;
		uint32_t	m_prevFb;
    };
}