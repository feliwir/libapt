#include <libapt/manager.hpp>
#include "graphics/flextGL.hpp"
#include "default_fp.hpp"
#include "graphics/debug.hpp"
#include <vector>
#include <iostream>
using namespace libapt;
using namespace std::chrono_literals;

Manager::Manager() : m_fileprovider(nullptr), m_fps(30), m_debug(false),
m_width(800),m_height(600), m_dimChanged(false), m_x(0),m_y(0)
{
	if (flextInit() == GL_FALSE)
	{
		std::cout << "Unable to initialize flextGL! Check for OGL Context" << std::endl;
	}
	Debug::Initialize();
	if (FLEXT_KHR_debug)
	{
		#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Debug::Callback, nullptr);
		#endif
	}

	m_fileprovider = std::make_shared<DefaultFp>();
	m_target = std::make_unique<RenderTarget>();
	m_target->SetDimension(m_width, m_height);
	m_last = std::chrono::system_clock::now();
}

Manager::Manager(std::shared_ptr<IFileProvider> fp) : m_fileprovider(fp),  m_fps(30),
m_width(800), m_height(600), m_x(0), m_y(0), m_debug(false)
{
	if (flextInit() == GL_FALSE)
	{
		std::cout << "Unable to initialize flextGL! Check for OGL Context" << std::endl;
	}
	Debug::Initialize();
	if (FLEXT_KHR_debug)
	{
		#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Debug::Callback, nullptr);
		#endif
	}

	m_target = std::make_unique<RenderTarget>();
	m_target->SetDimension(m_width, m_height);
	m_last = std::chrono::system_clock::now();
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

	return NO_ERROR;
}

uint32_t Manager::GetFramebuffer()
{
	return m_target->GetFramebuffer();
}

void Manager::Render(const bool window)
{
	if (m_dimChanged)
	{
		glViewport(0, 0, m_width, m_height);
		UpdateDimensions();
	}
		
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	m_target->Bind();
	
	auto now = std::chrono::system_clock::now();
	auto timePassed = (now - m_last);
	if (timePassed > (1000ms/m_fps))
	{	
		auto rest = timePassed - (1000ms/m_fps);
		glClear(GL_COLOR_BUFFER_BIT);
		m_active->Render();
		m_last = now+rest;

		if (m_dimChanged)
			m_dimChanged = false;
	}
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (window)
		m_target->Render();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}

void Manager::SetFps(const uint32_t fps)
{
	m_fps = fps;
}

void Manager::SetDebug(const bool debug)
{
	m_debug = debug;
}

void Manager::SetWidth(const uint32_t width)
{
	m_dimChanged = true;
	m_width = width;
}

void Manager::SetHeight(const uint32_t height)
{
	m_dimChanged = true;
	m_height = height;
}

uint32_t Manager::GetWidth()
{
	return m_width;
}

uint32_t Manager::GetHeight()
{
	return m_height;
}

void Manager::UpdateDimensions()
{
	m_target->SetDimension(m_width, m_height);
}

void Manager::SetMousePosition(double x, double y)
{
	m_x = x;
	m_y = y;
}

void Manager::GetMousePosition(double & x, double & y)
{
	x = m_x;
	y = m_y;
}
