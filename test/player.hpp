#pragma once
#include <GLFW/glfw3.h>
#include <libapt/manager.hpp>
#include <memory>
#include <string>

class Player
{
public:
	Player();
	~Player();

	//setters
	bool SetInput(const std::string& name);
	void SetFps(const unsigned int fps);
	void SetWidth(const unsigned int width);
	void SetHeight(const unsigned int width);
	//run
	void Run();
private:
	static void Error(int error, const char* description);
private:
	GLFWwindow* m_window;
	std::shared_ptr<libapt::Manager> m_mngr;
	std::string m_input;
};