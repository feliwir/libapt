#pragma once
#include <libapt/fileprovider.hpp>

namespace libapt
{
	class DefaultFp : public IFileProvider
	{
	public:
		virtual const uint8_t* LoadBinary(const std::string& name,unsigned int& size);
		virtual const std::string LoadText(const std::string& name,bool& fail);
		virtual const std::shared_ptr<Texture> LoadTexture(const std::string& name);
	};
}