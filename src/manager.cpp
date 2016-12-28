#include <libapt/manager.hpp>
#include "graphics/flextGL.h"
#include "default_fp.hpp"
#include "debug.hpp"
#include <vector>
#include <iostream>

using namespace libapt;

Manager::Manager() : m_fileprovider(nullptr), m_useFb(false), m_fps(30)
{
	if (flextInit() == GL_FALSE)
	{
		std::cout << "Unable to initialize flextGL! Check for OGL Context" << std::endl;
	}
	if (FLEXT_KHR_debug)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debugCallback, nullptr);
	}

	m_fileprovider = std::make_shared<DefaultFp>();
	m_target = std::make_unique<RenderTarget>();
}

Manager::Manager(std::shared_ptr<IFileProvider> fp) : m_fileprovider(fp), m_useFb(false), m_fps(30)
{
	if (flextInit() == GL_FALSE)
	{
		std::cout << "Unable to initialize flextGL! Check for OGL Context" << std::endl;
	}
	if (FLEXT_KHR_debug)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debugCallback, nullptr);
	}

	m_target = std::make_unique<RenderTarget>();
}

Error Manager::AddApt(const std::string& name)
{
	unsigned int size = 0;
	Error err = NO_ERROR;

	//Load the apt with const, dat etc.	
	const uint8_t* buffer = m_fileprovider->LoadBinary(name+".apt",size);
	if (buffer == nullptr)
		return APT_NF;

	auto aptFile = std::make_shared<Apt>();
	auto this_ptr = shared_from_this();
	err = aptFile->Load(buffer, size,this_ptr,name);
	if (err != NO_ERROR)
	{
		return err;
	}

	m_apts[name] = aptFile;
	return err;
}

std::shared_ptr<Character> Manager::ImportCharacter(const std::string& movie, const std::string& name)
{
	auto apt = GetApt(movie);
	if (apt==nullptr)
		return nullptr;

	auto ch = apt->GetExport(name);
	if (ch == nullptr)
		std::cout << "Failed to import character named: " << name << " from movie: " << movie << std::endl;
	return ch;
}

std::shared_ptr<Apt> Manager::GetApt(const std::string& name)
{
	auto aptFile = m_apts[name];
	if (aptFile == nullptr)
	{
		if (AddApt(name) != NO_ERROR)
		{
			std::cout << "Failed to load: " << name << ".apt" << std::endl;
			return nullptr;
		}
		else
		{
			std::cout << "Succesfully loaded: " << name << ".apt" << std::endl;
			aptFile = m_apts[name];
		}
			
	}
	return aptFile;
}

Error Manager::SetActive(const std::string& name)
{
	m_active = m_apts[name];
	if (m_active == nullptr)
		return INVALID_APT;

	m_target->SetDimension(m_active->GetWidth(), m_active->GetHeight());
	return NO_ERROR;
}

void Manager::UseFramebuffer(bool useFb)
{
	m_target->Bind();
	m_useFb = useFb;
}

uint32_t Manager::GetFramebuffer()
{
	return m_target->GetFramebuffer();
}

void Manager::Render()
{
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	if (m_useFb)
		m_target->Bind();
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_active->Render();

	if(m_useFb)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Manager::Update()
{

}

void Manager::SetFps(uint32_t fps)
{
	m_fps = fps;
}