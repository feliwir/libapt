#include <libapt/rendertarget.hpp>
#include <iostream>
#include "flextGL.h"
using namespace libapt;

RenderTarget::RenderTarget() : m_fbId(0), m_texId(0), m_dbId(0)
{
	glGenFramebuffers(1, &m_fbId);
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenRenderbuffers(1, &m_dbId);
}

bool RenderTarget::SetDimension(uint32_t width, uint32_t height)
{
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, m_dbId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_dbId);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texId, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Can't setup framebuffer!" << std::endl;
		return false;
	}

	return true;
}

RenderTarget::~RenderTarget()
{
	if (m_dbId)
	{
		glDeleteRenderbuffers(1, &m_dbId);
		m_dbId = 0;
	}
	if (m_fbId)
	{
		glDeleteFramebuffers(1, &m_fbId);
		m_fbId = 0;
	}
	if (m_texId)
	{
		glDeleteTextures(1, &m_texId);
		m_texId = 0;
	}
}

void RenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbId);
}