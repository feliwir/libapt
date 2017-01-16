#pragma once
#include <string>
#include <memory>

namespace libapt
{
    namespace as
    {
		class Object;

        struct Function
        {
			std::string Name;
			uint32_t nParams;
			std::vector<std::string> Params;
			uint32_t Size;
			uint8_t* Code;
			uint32_t nRegisters;
			std::shared_ptr<Object> Owner;
        };

		struct Argument
		{
			uint32_t Register;
			std::string Name;
		};
    }
}