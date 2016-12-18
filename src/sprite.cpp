#include "sprite.hpp"
#include "util.hpp"
#include <libapt/apt.hpp>
using namespace libapt;


void Sprite::Parse(uint8_t *& iter)
{
	m_framecount = read<uint32_t>(iter);
	uint8_t* frameOffset = const_cast<uint8_t*>(m_owner->GetBase()) + read<uint32_t>(iter);

	for (uintptr_t i = 0;i < m_framecount;++i)
	{
		Frame f;
		f.Parse(frameOffset, m_owner->GetBase());
		frameOffset += Frame::FRAMESIZE;
		m_frames.push_back(f);
	}
}
