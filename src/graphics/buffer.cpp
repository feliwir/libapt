#include "buffer.hpp"
#include "flextGL.h"
using namespace libapt;

Buffer::Buffer() : m_vbo(0)
{
	glGenBuffers(1, &m_vbo);
}

Buffer::~Buffer()
{
	if (m_vbo)
		glDeleteBuffers(1, &m_vbo);
}

uint32_t Buffer::Append(const std::vector<glm::vec2>& verts)
{
	uint32_t index = m_vertices.size();
	m_vertices.insert(m_vertices.end(), verts.begin(),verts.end());
	return index;
}

void Buffer::Finalize()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
	m_vertices.clear();
}

void Buffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}
