#pragma once
#include "texture.hpp"
#include <memory>
#include <string>

namespace libapt
{
    class IFileProvider
    {
	public:
		virtual const uint8_t* LoadBinary(const std::string& name,unsigned int& size) = 0;
		virtual const std::string LoadText(const std::string& name,bool& fail) = 0;
    };
}