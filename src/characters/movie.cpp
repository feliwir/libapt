#include "movie.hpp"
#include <libapt/apt.hpp>
#include "../util.hpp"
#include <iostream>
using namespace libapt;

void Movie::Parse(uint8_t *& iter)
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

	m_unknown = read<uint32_t>(iter);
	m_characterCount = read<uint32_t>(iter);

	uint8_t* charsOffset = const_cast<uint8_t*>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (uintptr_t i = 0;i < m_characterCount;++i)
	{
		uint32_t relOffset =  read<uint32_t>(charsOffset);
		if (relOffset && relOffset!=m_thisOffset)
		{
			uint8_t* iter = const_cast<uint8_t*>(m_owner->GetBase()) + relOffset;
			auto ch = Character::Create(iter, m_owner);
			m_dictionary.push_back(ch);
		}
		else if (relOffset && relOffset == m_thisOffset)
		{
			m_dictionary.push_back(shared_from_this());
		}
		else
		{
			m_dictionary.push_back(nullptr);
		}
	}

	m_screenwidth = read<uint32_t>(iter);
	m_screenheight = read<uint32_t>(iter);
	m_unknown2 = read<uint32_t>(iter);	
	m_importcount = read<uint32_t>(iter);
	uint8_t* importOffset = const_cast<uint8_t*>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (uintptr_t i = 0;i < m_importcount;++i)
	{
		Import im;
		im.Parse(importOffset, m_owner->GetBase());
		m_imports.push_back(im);
	}
	m_exportcount = read<uint32_t>(iter);
	uint8_t* exportOffset = const_cast<uint8_t*>(m_owner->GetBase()) + read<uint32_t>(iter);
	for (uintptr_t i = 0;i < m_exportcount;++i)
	{
		Export ex;
		ex.Parse(exportOffset, m_owner->GetBase());
		m_exports.push_back(ex);
	}
}

void Movie::SetCharacter(const std::shared_ptr<Character> ch, uint32_t index)
{
	if (index > m_dictionary.size())
	{
		std::cout << "Can't set character, because index is out of range" << std::endl;
		return;
	}
	m_dictionary[index] = ch;
}
