#include "initaction.hpp"
#include "util.hpp"
#include <libapt/bytecode.hpp>
using namespace libapt;

void InitAction::Parse(uint8_t *offset, const uint8_t *base)
{
	m_sprite = read<uint32_t>(offset);
	m_bytes = const_cast<uint8_t*>(read<uint32_t>(offset) + base);
}