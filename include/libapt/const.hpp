#pragma once
#include <string>
#include <vector>
#include <stdint.h>
#include "error.hpp"
#include "value.hpp"


namespace libapt
{
	class Const
	{
	private:
		enum Type
		{
			UNDEF = 0,
			STRING = 1,
			NUMBER = 4
		};
	public:
		Const();
		Error Load(const uint8_t* data, unsigned int size);

		inline Value GetItem(uint32_t index)
		{
			return m_items[index];
		}

		inline uint32_t GetAptOffset()
		{
			return m_aptOffset;
		}

		inline uint32_t GetItemCount()
		{
			return m_itemCount;
		}

	private:
		uint32_t m_aptOffset;
		uint32_t m_itemCount;
		std::vector<Value> m_items;
	};
}