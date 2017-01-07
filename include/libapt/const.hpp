#pragma once
#include "error.hpp"
#include <string>
#include <vector>
#include <stdint.h>

namespace libapt
{
	class Const
	{
	public:
		enum Type
		{
			UNDEF = 0,
			STRING = 1,
			NUMBER = 4
		};

		struct Entry
		{
			Type type;
			std::string stringVal;
			uint32_t numVal;
			Entry();
		};
	public:
		Const();
		Error Load(const uint8_t* data, unsigned int size);

		inline Entry GetItem(const uint32_t index)
		{
			if (index > m_items.size())
				return Entry();

			return m_items[index];
		}

		inline const uint32_t GetAptOffset()
		{
			return m_aptOffset;
		}

		inline const uint32_t GetItemCount()
		{
			return m_itemCount;
		}

	private:
		uint32_t m_aptOffset;
		uint32_t m_itemCount;
		std::vector<Entry> m_items;
	};
}