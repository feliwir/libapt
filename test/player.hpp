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
	void SetWidth(const unsigned int width,bool set = true);
	void SetHeight(const unsigned int width, bool set = true);
	//run
	void Run();
private:
	static void Error(int error, const char* description);
	static void Resize(GLFWwindow* win, int width, int height);
private:
	GLFWwindow* m_window;
	std::shared_ptr<libapt::Manager> m_mngr;
	std::string m_input;
};