#include <libapt/const.hpp>
#include "util.hpp"
#include <iostream>
using namespace libapt;

Const::Const() : m_aptOffset(0), m_itemCount(0)
{
}

Error Const::Load(const uint8_t* buffer, unsigned int size)
{
	uint8_t* iter = const_cast<uint8_t*>(buffer);
	//Skip the first 0x14 bytes
	std::string magic = readString(iter, 0x11);
	if (magic != "Apt constant file")
		return INVALID_CONST;

	iter += 0x03;
	m_aptOffset = read<uint32_t>(iter);
	m_itemCount = read<uint32_t>(iter);
	iter += 4;

	for (uint32_t i = 0;i < m_itemCount;++i)
	{
		Entry e;
		e.type = static_cast<Type>(read<uint32_t>(iter));
		uint32_t value = read<uint32_t>(iter);

		switch (e.type)
		{
		case STRING:
			e.stringVal = std::string(reinterpret_cast<const char*>(buffer + value));
			break;
		case NUMBER:
			e.numVal = value;
			break;
		case UNDEF:
			std::cout << "Undefined constant entry at position " << i << std::endl;
			break;
		default:
			break;
		}

		m_items.push_back(e);
	}
	
	return NO_ERROR;
}