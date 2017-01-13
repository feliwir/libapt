#pragma once
#include <libbig/manager.hpp>
#include <libapt/fileprovider.hpp>

class FileProvider : public libapt::IFileProvider
{
public:
	FileProvider();

	// Derived from IFileProvider
	virtual const uint8_t * LoadBinary(const std::string & name, unsigned int & size) override;
	virtual const std::string LoadText(const std::string & name, bool & fail) override;
private:
	libbig::Manager m_mngr;
};