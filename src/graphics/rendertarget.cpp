#include <libapt/rendertarget.hpp>
#include "shader.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include "flextGL.h"
using namespace libapt;

static Shader quad_shader;
static const std::string quad_vertSrc =
"#version 330\n"
"layout(location = 0)in vec2 vert;\n"
"out vec2 uv;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(vert,0, 1);\n"
"uv = (vert.xy + vec2(1, 1)) / 2.0;\n"
"}";

static const std::string quad_fragSrc =
"#version 330\n"
"in vec2 uv;\n"
"out vec4 fragColor;\n"
"uniform sampler2D tex;\n"
"void main()\n"
"{\n"
"fragColor = texture(tex,uv);\n"
"}";

static const GLfloat quad_buffer[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f,  1.0f,
	-1.0f,  1.0f,
	1.0f, -1.0f,
	1.0f,  1.0f,
};

static bool quad_initialized = false;
static GLuint quad_vao = 0;
static GLuint quad_vbo = 0;

RenderTarget::RenderTarget() : m_fbId(0), m_texId(0), m_dbId(0),
m_width(0), m_height(0)
{
	if (!quad_initialized)
	{
		quad_shader.Load(quad_vertSrc, quad_fragSrc);
		glGenVertexArrays(1, &quad_vao);
		glBindVertexArray(quad_vao);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
		glGenBuffers(1, &quad_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_buffer), quad_buffer, GL_STATIC_DRAW);
		quad_initialized = true;
	}

	glGenFramebuffers(1, &m_fbId);
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenRenderbuffers(1, &m_dbId);
}

bool RenderTarget::SetDimension(uint32_t width, uint32_t height)
{
	if (width == m_width && height == m_height)
		return true;

	m_width = width;
	m_height = height;
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

void RenderTarget::Render()
{
	quad_shader.Use();
	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}