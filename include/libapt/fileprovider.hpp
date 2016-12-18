#pragma once
#include <memory>
#include <string>
#include "texture.hpp"

namespace libapt
{
    class IFileProvider
    {
	public:
		virtual const uint8_t* LoadBinary(const std::string& name,unsigned int& size) = 0;
		virtual const std::string LoadText(const std::string& name,bool& fail) = 0;
		virtual const std::shared_ptr<Texture> LoadTexture(const std::string& name) = 0;
    };
}