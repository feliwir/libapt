#include <libapt/manager.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

static void error_callback(int error, const char* description)
{
	std::cerr <<  "Error: " << description << std::endl;
}

int main(int argc, char** argv)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return EXIT_FAILURE;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Apt player", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Unable to create window" << std::endl;
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwShowWindow(window);
	int frame = 0;
	std::shared_ptr<libapt::Manager> mngr = std::make_shared<libapt::Manager>();
	mngr->AddApt("MainMenu");
	mngr->SetActive("MainMenu");
	while (!glfwWindowShouldClose(window))
	{
		std::cout << "--Current frame: " << frame << std::endl;
		glClear(GL_COLOR_BUFFER_BIT);
		mngr->Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
		std::this_thread::sleep_for(1s);
		frame++;
	}
	return 0;
}