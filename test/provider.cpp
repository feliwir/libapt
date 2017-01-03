#include "provider.hpp"
#include <iostream>
#ifdef _WIN32
#include <filesystem>
namespace fs = std::tr2::sys;
#else
namespace fs = boost::fs;
#endif
FileProvider::FileProvider()
{
	//get all big archives in workdir 
	fs::recursive_directory_iterator it(".");
	fs::recursive_directory_iterator end;
	std::vector<std::string> archives;

	while (it != end)
	{
		if (fs::is_regular_file(*it) && it->path().extension() == ".big") 
			archives.push_back(it->path().filename().string());
		++it;

	}

	for(const auto& a : archives)
		m_mngr.AddBig(a);

}

const uint8_t * FileProvider::LoadBinary(const std::string & name, unsigned int & size)
{
	uint8_t* binary = m_mngr.GetEntry(name, size);
	
	if (binary == nullptr)
	{
		std::cout << "Failed to load: " << name << std::endl;
	}

	return binary;
}

const std::string FileProvider::LoadText(const std::string & name, bool & fail)
{
	fail = false;
	const auto& file = m_mngr.GetEntry(name);
	
	if (file.size() == 0)
	{
		fail = true;
		std::cout << "Failed to load: " << name << std::endl;
	}
		
	return file;
}
