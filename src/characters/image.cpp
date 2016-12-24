#include "image.hpp"
#include "../util.hpp"
#include <iostream>
using namespace libapt;

void Image::Parse(uint8_t *& iter)
{
	m_texture = read<uint32_t>(iter);
}

void Image::Update(const Transformation& t, DisplayObject& dObj)
{
	std::cout << "WARNING: Image should not be able to be on displaylist" << std::endl;
}
