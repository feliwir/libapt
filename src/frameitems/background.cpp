#include "background.hpp"
#include "../util.hpp"
using namespace libapt;

void Background::Parse(uint8_t * iter, const uint8_t * base)
{
	m_color = read<glm::u8vec4>(iter);
}
