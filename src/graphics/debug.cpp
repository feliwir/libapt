#include "debug.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace libapt;

glm::mat4 Debug::s_ortho;
Shader Debug::s_shader;
uint32_t Debug::s_vao = 0;
std::string Debug::s_vertSrc =
"#version 330\n"
"layout(location = 0)in vec2 vert;\n"
"uniform vec2 translate;\n"
"uniform mat2 rotscale;\n"
"uniform mat4 ortho;\n"
"void main()\n"
"{\n"
"   vec2 sc=rotscale*vert+translate;\n"
"	gl_Position = ortho*vec4(sc,0,1);\n"
"}";

std::string Debug::s_fragSrc =
"#version 330\n"
"out vec4 fragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"	fragColor = color;\n"
"}";

void Debug::Initialize()
{
	s_ortho = glm::ortho(0.0, static_cast<double>(1024),
		static_cast<double>(768), 0.0, 0.0, 10.0);
	if (!s_shader.Valid())
	{
		s_shader.Load(s_vertSrc, s_fragSrc);
		s_shader.addUniform("translate");
		s_shader.addUniform("rotscale");
		s_shader.addUniform("color");
		s_shader.addUniform("ortho");

		glGenVertexArrays(1, &s_vao);
		glBindVertexArray(s_vao);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
		glBindVertexArray(0);
	}
}

void Debug::Draw(const Buffer& b,const Transformation& t,const glm::vec4& color)
{
	s_shader.Use();
	glBindVertexArray(s_vao);
	b.Bind();
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		nullptr
	);
	glUniform4fv(s_shader.uniform("color"), 1, glm::value_ptr(color));
	glUniformMatrix4fv(s_shader.uniform("ortho"), 1, GL_FALSE, glm::value_ptr(s_ortho));
	glUniformMatrix2fv(s_shader.uniform("rotscale"), 1, GL_FALSE, glm::value_ptr(t.rotscale));
	glUniform2fv(s_shader.uniform("translate"), 1, glm::value_ptr(t.translate));
	glDrawArrays(GL_TRIANGLES, 0, b.GetSize());
}


void APIENTRY Debug::Callback(GLenum source, GLenum type, GLuint id,
			GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
{
	std::cout << message << std::endl;
}