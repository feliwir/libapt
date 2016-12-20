#include "action.hpp"
#include "../util.hpp"
using namespace libapt;

void Action::Parse(uint8_t *offset, const uint8_t *base)
{
	m_bytes = base + read<uint32_t>(offset);
}