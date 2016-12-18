#include "image.hpp"
#include "util.hpp"
using namespace libapt;

void Image::Parse(uint8_t *& iter)
{
	m_texture = read<uint32_t>(iter);
}

void Image::Update(Transformation t)
{
	//Don't render anything
}
