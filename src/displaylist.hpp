#pragma once
#include "displayobject.hpp"
#include <map>
#include <string>
#include <stdint.h>

namespace libapt
{
	class DisplayList
	{
	public:
		void Insert(uint32_t depth, std::shared_ptr<Character> ch, 
			const glm::vec2& translate,	const glm::mat2& rotscale, 
			const std::string& name, std::shared_ptr<Container> parent);

		void AddClipLayer(uint32_t depth, uint32_t clipdepth, std::shared_ptr<Character> ch,
			const glm::vec2& translate, const glm::mat2& rotscale,
			const std::string& name, std::shared_ptr<Container> parent);

		void Erase(uint32_t depth);

		void Move(uint32_t depth, const glm::vec2& translate,
			const glm::mat2& rotscale);

		void Colortransform(uint32_t depth,glm::u8vec4 color);

		void Render(const Transformation& t);

		DisplayObject& GetObject(uint32_t depth)
		{
			return m_objects[depth];
		}
	private:
		std::map<uint32_t, DisplayObject> m_objects;
	};
}