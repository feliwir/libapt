#include "default_fp.hpp"
#include <fstream>
#include <iostream>
using namespace libapt;

const uint8_t* DefaultFp::LoadBinary(const std::string& name,unsigned int& size)
{
	std::ifstream fin(name,std::ios::binary);
	if (fin.fail())
	{
		std::cout << "Failed to load: " << name << std::endl;
		return nullptr;
	}
	fin.seekg(0, std::ios::end);
	size = fin.tellg();
	fin.seekg(0, std::ios::beg);

	uint8_t* buffer = new uint8_t[size];
	fin.read(reinterpret_cast<char*>(buffer), size);

	return buffer;
}

const std::string DefaultFp::LoadText(const std::string& name,bool& fail)
{
	std::ifstream t(name);
	fail = false;
	if (t.fail())
	{
		fail = true;
		return "";
	}
		
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return str;
}