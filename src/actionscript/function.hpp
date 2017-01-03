#pragma once
#include <string>
#include <vector>

namespace libapt
{
    namespace as
    {
        struct Function
        {
			std::string Name;
			uint32_t nParams;
			std::vector<std::string> Params;
			uint32_t Size;
			uint8_t* Code;
        };

		struct Argument
		{
			uint32_t Register;
			std::string Name;
		};
    }
}