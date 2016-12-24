#pragma once
#include <string>
#include <vector>

namespace libapt
{
    namespace as
    {
        struct Function
        {
			std::string name;
			uint32_t nParams;
			std::vector<std::string> params;
			uint32_t size;
			uint8_t* bc;
        };
    }
}