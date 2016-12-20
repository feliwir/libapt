#include "removeobject.hpp"
#include "../util.hpp"
using namespace libapt;

void RemoveObject::Parse(uint8_t *offset, const uint8_t *base)
{
	m_depth = read<int32_t>(offset);
}