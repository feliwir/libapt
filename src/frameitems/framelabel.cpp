#include "framelabel.hpp"
#include "../util.hpp"
using namespace libapt;

void Framelabel::Parse(uint8_t *offset, const uint8_t *base)
{
	uint8_t* lblOffset = const_cast<uint8_t*>(read<uint32_t>(offset) + base);
	m_label = readString(lblOffset);
	m_flags = read<uint32_t>(offset);
	m_frameid = read<uint32_t>(offset);
}
