#include <libapt/texture.hpp>
#include "flextGL.h"
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
