#pragma once
#include "character.hpp"

namespace libapt
{
	class Image : public Character
	{
	public:
		virtual void Parse(uint8_t*& iter);
		inline uint32_t GetImage() { return m_image; }
		virtual void Update(const Transformation& t, DisplayObject& dObj);
	private:
		uint32_t m_image;
	};
}