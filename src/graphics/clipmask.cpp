#include "clipmask.hpp"
#include "flextGL.h"
#include <iostream>
using namespace libapt;

ClipMask::ClipMask(uint32_t w,uint32_t h) : m_fbId(0), m_texId(0), m_dbId(0),
m_width(w), m_height(h)
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, reinterpret_cast<GLint*>(&m_prevFb));
	glGenFramebuffers(1, &m_fbId);
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenRenderbuffers(1, &m_dbId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindRenderbuffer(GL_RENDERBUFFER, m_dbId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_dbId);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texId, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Can't setup framebuffer!" << std::endl;
	}

	UnbindFb();
}

ClipMask::~ClipMask()
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

void ClipMask::BindFb()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING,reinterpret_cast<GLint*>(&m_prevFb));
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbId);

}

void ClipMask::UnbindFb()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_prevFb);
}

void ClipMask::BindMask()
{
	glBindTexture(GL_TEXTURE_2D, m_texId);
}

void ClipMask::Clear()
{
	GLfloat clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR);
	glClearColor(clearColor[0], clearColor[1], 
				clearColor[2], clearColor[3]);
}