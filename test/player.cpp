#include "player.hpp"
#include "provider.hpp"
#include <iostream>

void Player::Error(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

Player::Player()
{
	glfwSetErrorCallback(Error);
	if (glfwInit() == GLFW_FALSE)
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GL_TRUE);
	#else
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif

	m_window = glfwCreateWindow(800, 600, "Apt player", NULL, NULL);
	if (m_window == NULL)
		exit(EXIT_FAILURE);

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(0);
	
	m_mngr = std::make_shared<libapt::Manager>(std::make_shared<FileProvider>());
}

void Player::Run()
{
	libapt::Error result = libapt::NO_ERROR;
	m_mngr->UpdateDimensions();
	result = m_mngr->SetActive(m_input);

	if (result != libapt::NO_ERROR)
	{
		std::cout << "Could not run apt player! Error code: " << result << std::endl;
		return;
	}

	glfwShowWindow(m_window);

	while (!glfwWindowShouldClose(m_window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		m_mngr->Render(true);
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

Player::~Player()
{
	glfwTerminate();
}

bool Player::SetInput(const std::string& name)
{
	m_input = name;

	if (m_mngr->AddApt(name) != libapt::NO_ERROR)
		return false;

	return true;
}

void Player::SetFps(const unsigned int fps)
{
	m_mngr->SetFps(fps);
}

void Player::SetWidth(const unsigned int width)
{
	glfwSetWindowSize(m_window, width, m_mngr->GetHeight());
	m_mngr->SetWidth(width);
}

void Player::SetHeight(const unsigned int height)
{
	glfwSetWindowSize(m_window, m_mngr->GetWidth(), height);
	m_mngr->SetHeight(height);
}
