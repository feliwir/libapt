#include "import.hpp"
#include "util.hpp"
using namespace libapt;

void Import::Parse(uint8_t*& iter, const uint8_t* base)
{
	uint32_t strOffset = read<uint32_t>(iter);
	if (strOffset)
		m_movie = readString(base + strOffset);
	strOffset = read<uint32_t>(iter);
	if (strOffset)
		m_name = readString(base + strOffset);

	m_character = read<uint32_t>(iter);
	m_pointer = read<uint32_t>(iter);
}