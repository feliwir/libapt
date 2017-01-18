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

		void Bind() const;

		inline uint32_t GetSize() const
		{
			return m_size;
		}
	private:
		uint32_t m_vbo;
		std::vector<glm::vec2> m_vertices;
		uint32_t m_size;
	};
}