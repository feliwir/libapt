#include "frame.hpp"
#include "util.hpp"
using namespace libapt;

void Frame::Parse(const uint8_t* offset, const uint8_t* base)
{
	uint8_t* iter = const_cast<uint8_t*>(offset);
	m_itemCount = read<uint32_t>(iter);
	uint8_t* itemsOffset = const_cast<uint8_t*>(base) + read<uint32_t>(iter);

	for (uintptr_t i = 0;i < m_itemCount;++i)
	{
		uint8_t* itemOffset = const_cast<uint8_t*>(base) + read<uint32_t>(itemsOffset);
		m_items.push_back(FrameItem::Create(itemOffset, base));
	}
}
