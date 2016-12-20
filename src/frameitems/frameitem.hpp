#pragma once
#include <stdint.h>
#include <memory>
#include <vector>

namespace libapt
{
	class FrameItem
	{
	public:
		enum Type
		{
			ACTION = 1,
			FRAMELABEL = 2,
			PLACEOBJECT = 3,
			REMOVEOBJECT = 4,
			BACKGROUNDCOLOR = 5,
			INITACTION = 8
		};

		virtual void Parse(uint8_t *offset, const uint8_t *base) = 0;
		static std::shared_ptr<FrameItem> Create(uint8_t*& iter, const uint8_t* base);
		inline Type GetType() { return m_type; }
		inline void SetType(const Type t) { m_type = t; }
	private:
		Type m_type;
	};

}