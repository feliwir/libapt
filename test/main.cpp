#include <iostream>
#include <cxxopts.hpp>
#include "player.hpp"
#include "provider.hpp"

int main(int argc, char** argv)
{
	//option parsing
	cxxopts::Options options("Apt player", "A program to playback EA apt files");
	options.add_options()
		("s,speed", "Set the speed in fps", cxxopts::value<unsigned int>())
		("f,file", "File name", cxxopts::value<std::string>())
		("w,width","Width of the window", cxxopts::value<unsigned int>())
		("h,height","Height of the window", cxxopts::value<unsigned int>());

	options.parse(argc, argv);
	if (options.count("file") == 0)
	{
		std::cout << "Please specify an input file." << std::endl << options.help() << std::endl;
		return EXIT_FAILURE;
	}

	Player p;
	//Set input file
	p.SetInput(options["file"].as<std::string>());
	//Set the fps
	if (options.count("speed") > 0)
		p.SetFps(options["speed"].as<unsigned int>());
	if (options.count("width") > 0)
		p.SetWidth(options["width"].as<unsigned int>());
	if (options.count("height") > 0)
		p.SetHeight(options["height"].as<unsigned int>());

	p.Run();
	return 0;
}