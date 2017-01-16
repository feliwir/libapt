#pragma once
#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>

namespace libapt
{
	class Buffer
	{
	public:
		Buffer();
		~Buffer();

		uint32_t Append(const std::vector<glm::vec2>& verts);
		void Finalize();

		void Bind();
	private:
		uint32_t m_vbo;
		std::vector<glm::vec2> m_vertices;
	};
}