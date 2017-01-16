#pragma once
#include "graphics/shader.hpp"
#include "characters/transformation.hpp"
#include <libapt/error.hpp>
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace libapt
{
	class Apt;
	class Texture;
	class ClipMask;

	class Geometry
	{
	private:
		enum Style
		{
			SOLID_TRIS = 0,
			TEXTURED_TRIS = 1,
			LINES = 2,
			UNDEFINED = 3
		};

		struct Triangle
		{
			glm::vec2 v1;
			glm::vec2 v2;
			glm::vec2 v3;
		};

		struct Line
		{
			glm::vec2 v1;
			glm::vec2 v2;
		};

		struct Entry
		{
			Style style;
			glm::u8vec4 color;
			int32_t image;
			glm::mat2 rotscale;
			glm::vec2 translate;
			uint32_t thickness;
			std::vector<Line> lines;
			std::vector<Triangle> triangles;

			Entry();
		};

		struct Object
		{
			uint32_t numVerts;
			//uniforms
			bool textured;
			glm::vec4 color;
			glm::mat2 rotscale;
			glm::vec2 translate;
			std::shared_ptr<Texture> texture;
			//start in the vbo
			uint32_t start;

			Object();
		};
	public:
		Geometry(uint32_t width,uint32_t height);
		Error Load(const std::string& data);
		void Compile(std::shared_ptr<Apt> apt);
		void Draw(const Transformation& trans, std::shared_ptr<Apt> apt);
	private:
		void AddLines(std::vector<Line>& lines, glm::u8vec4 color, int thickness);
		void AddSolidTriangles(std::vector<Triangle>& tris, glm::u8vec4 color);
		void AddTexturedTriangles(std::vector<Triangle>& tris, glm::u8vec4 color, uint32_t image,
			glm::mat2 rotscale, glm::vec2 translate);
		
		std::vector<Triangle> TriangulateLine(Line l, uint32_t thickness);
	private:
		std::vector<Entry> m_entries;
		std::vector<Object> m_objects;
		glm::mat4 m_ortho;
		//Shader stuff
		static Shader s_shader;
		static std::string s_vertSrc;
		static std::string s_fragSrc;
		static uint32_t s_vao;
		 
	};
}