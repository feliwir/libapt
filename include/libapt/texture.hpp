#pragma once
#include <stdint.h>

namespace libapt
{
	class Texture
	{		
	public:
		enum Format
		{
			RGB = 0,
			RGBA = 1,
		};

		Texture();
		~Texture();

		void Load(const uint8_t* buffer, uint32_t size);
		void Create(uint32_t width, uint32_t height, Format format);
		//RGBA buffer
		void Update(const uint8_t* data);

		void Bind();
	private:
		uint32_t m_texId;
		uint32_t m_width;
		uint32_t m_height;
		Format m_format;
	};
}