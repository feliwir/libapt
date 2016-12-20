#include <libapt/dat.hpp>
#include <sstream>
#include "util.hpp"
using namespace libapt;

Error Dat::Load(const std::string & data)
{
	std::stringstream filestream(data);
	std::stringstream paramstream;
	std::string line;
	int pos = 0;
	int charId;
	Entry e;

	//Get line by line
	while (std::getline(filestream, line))
	{
		//Parse the line
		trim(line);
		pos = line.find(";");
		if (pos != std::string::npos)
			line = line.substr(0, pos);

		//Only get the things before a comment
		if (line.size() == 0)
			continue;

		pos = line.find("->");
		if (pos == std::string::npos)
		{
			pos = line.find("=");
			if (pos == std::string::npos)
				return INVALID_DAT;

			e.type = HAS_BOUND;
			std::string imgId = line.substr(pos + 1, line.size());
			paramstream.str(line.substr(pos + 1, line.size()));
		}
		else
		{
			e.type = NO_BOUNDS;
			std::string imgId = line.substr(pos + 2, line.size());
			e.p1 = std::stoi(imgId);
		}
			
		charId = std::stoi(line.substr(0, pos));
		paramstream >> e.p1;
		if (e.type == HAS_BOUND)
		{
			paramstream >> e.p1 >> e.p2 >> e.p3 >> e.p4;
		}
		m_entries[charId] = e;
		e.p1 = e.p2 = e.p3 = e.p4 = 0;

	}

	return NO_ERROR;
}
