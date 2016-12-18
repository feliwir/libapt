#pragma once
#include <stdint.h>

namespace libapt
{
	class RenderTarget
	{
	public:
		RenderTarget();
		~RenderTarget();

		bool SetDimension(uint32_t width, uint32_t height);
		void Bind();
		inline uint32_t GetFramebuffer()
		{
			return m_fbId;
		}
	private:
		uint32_t	m_fbId;
		uint32_t	m_texId;
		uint32_t	m_dbId;
	};
}