#include <libapt/texture.hpp>
#include "targa.hpp"
#include "../util.hpp"
#include "flextGL.h"
#include <gli/gli.hpp>
using namespace libapt;

Texture::Texture() : m_texId(0), m_width(0), m_height(0)
{
	glGenTextures(1, &m_texId);
}

Texture::~Texture()
{
	if (m_texId)
	{
		glDeleteTextures(1, &m_texId);
		m_texId = 0;
	}
}

void Texture::Create(uint32_t width, uint32_t height,Format format)
{
	m_width = width;
	m_height = height;
	m_format = format;
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::Update(const uint8_t* data)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texId);
}

void Texture::Load(const uint8_t* buffer, uint32_t size)
{
	Bind();

	//try dds first
	gli::texture tex = gli::load_dds(reinterpret_cast<const char*>(buffer), size);
	//do tga loading
	if (tex.empty())
	{
		GLuint format = GL_RGB;
		Targa tga;
		uint8_t* iter = const_cast<uint8_t*>(buffer);

		for(uint32_t b = 0;b<12;++b)
			tga.Header[b] = read<uint8_t>(iter);

		tga.Width = read<uint16_t>(iter);
		tga.Height = read<uint16_t>(iter);
		tga.Bpp = read<uint8_t>(iter);
		tga.Id = read<uint8_t>(iter);	
		tga.Data = iter;

		switch (tga.Bpp)
		{
		case 24:
			format = GL_RGB;
			break;
		case 32:
			format = GL_BGRA;
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tga.Width, tga.Height, 0, format, GL_UNSIGNED_BYTE, tga.Data);
	}
	else
	{

	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}