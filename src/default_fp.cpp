#include "default_fp.hpp"
#define STBI_ONLY_JPEG
#define STBI_ONLY_TGA
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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

const std::shared_ptr<Texture> DefaultFp::LoadTexture(const std::string& name)
{
	int x, y, c;
	unsigned int size = 0;
	const unsigned char* buffer = LoadBinary(name, size);
	stbi_uc* img_buffer = stbi_load_from_memory(buffer, size, &x, &y, &c, STBI_default);
	std::shared_ptr<Texture> tex = std::make_shared<Texture>();
	tex->Create(x, y,Texture::RGBA);
	tex->Update(img_buffer);
	stbi_image_free(img_buffer);
	delete[] buffer;
	return tex;
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