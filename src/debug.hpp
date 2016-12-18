#pragma once

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
                           GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << message << std::endl;
}