#include "shader.hpp"
#include "flextGL.h"
#include <vector>
#include <iostream>
using namespace libapt;

Shader::Shader() : m_vert(0),m_frag(0),m_program(0)
{
}

Error Shader::Load(const std::string& vert, const std::string& frag)
{
	Cleanup();
	GLint success = 0;
	GLint logSize = 0;
	m_program = glCreateProgram();
	m_vert = glCreateShader(GL_VERTEX_SHADER);
	m_frag = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* vert_src = vert.c_str();
	const GLint vert_size = vert.size();
	glShaderSource(m_vert, 1, &vert_src, &vert_size);
	glCompileShader(m_vert);
	glGetShaderiv(m_vert, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(m_vert, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(m_vert, maxLength, &maxLength, &errorLog[0]);
		std::cout << errorLog.data() << std::endl;
		Cleanup();
		return INVALID_SHADER;
	}

	const GLchar* frag_src = frag.c_str();
	const GLint frag_size = frag.size();
	glShaderSource(m_frag, 1, &frag_src, &frag_size);
	glCompileShader(m_frag);
	glGetShaderiv(m_frag, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(m_frag, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(m_frag, maxLength, &maxLength, &errorLog[0]);
		std::cout << errorLog.data() << std::endl;
		Cleanup();
		return INVALID_SHADER;
	}

	glAttachShader(m_program, m_vert);
	glAttachShader(m_program, m_frag);
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(m_program, maxLength, &maxLength, &errorLog[0]);
		std::cout << errorLog.data() << std::endl;
		Cleanup();
		return INVALID_SHADER;
	}

	return NO_ERROR;
}

void Shader::Cleanup()
{
	if (m_program)
	{
		glDeleteProgram(m_program);
		m_program = 0;
	}
	if (m_vert)
	{
		glDeleteShader(m_vert);
		m_program = 0;
	}
	if (m_frag)
	{
		glDeleteShader(m_frag);
		m_program = 0;
	}
}

void Shader::Use()
{
	glUseProgram(m_program);
}

Shader::~Shader()
{
	Cleanup();
}

bool Shader::Valid()
{
	return m_program != 0;
}

void Shader::addUniform(const std::string & name)
{
	GLint attr = glGetUniformLocation(m_program, name.c_str());
	m_uniformLocationList[name] = attr;
}

void Shader::addAttribute(const std::string & name)
{
	GLint index = m_attributeList.size();
	glBindAttribLocation(m_program, index, name.c_str());
	m_attributeList[name] = index;
}

int32_t Shader::uniform(const std::string& name)
{

	return m_uniformLocationList[name];
}

int32_t Shader::attribute(const std::string& name)
{
	return m_attributeList[name];
}
