#pragma once
#include "error.hpp"
#include <string>
#include <memory>
#include <map>

namespace libapt
{
	class Manager;

    //contains a reference to all textures an apt needs
    class Dat
    {
	public:
		enum Type
		{
			NO_BOUNDS = 0,
			HAS_BOUND = 1
		};

		struct Entry
		{
			Type type;
			int p1,p2,p3,p4;
		};

		Error Load(const std::string& data);
		Entry GetEntry(const uint32_t charid)
		{
			return m_entries[charid];
		}
	private:
		std::map<int,Entry> m_entries;
    };
}