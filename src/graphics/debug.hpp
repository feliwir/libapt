#pragma once
#include <iostream>
#include "flextGL.h"
#include "shader.hpp"
#include "buffer.hpp"
#include "../characters/transformation.hpp"

namespace libapt
{
	class Debug
	{
	public:
		static void APIENTRY Callback(GLenum source, GLenum type, GLuint id,
			GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
		{
			std::cout << message << std::endl;
		}

		static void Initialize();

		static void Draw(const Buffer& buf,const Transformation& t, const glm::vec4& color);
	private:
		static uint32_t s_vao;
		static glm::mat4 s_ortho;
		static Shader s_shader;
		static std::string s_vertSrc;
		static std::string s_fragSrc;
	};
}