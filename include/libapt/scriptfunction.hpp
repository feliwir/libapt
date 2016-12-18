#pragma once
#include <stdint.h>
#include <string>
#include <vector>
namespace libapt
{
	class ScriptContext;

    class Function
    {
        public:
			struct Argument
			{
				uint32_t Register;
				std::string Name;
			};

            struct Flags
            {
                bool PreloadParent  : 1;
                bool PreloadRoot    : 1;
                bool SuppressSuper  : 1;
                bool PreloadSuper   : 1;
                bool SuppressArgs   : 1;
                bool PreloadArgs    : 1;
                bool SuppressThis   : 1;
                bool PreloadThis    : 1;
                bool Reserved		: 7;
                bool PreloadGlobal  : 1;
            };

			void Parse(uint8_t*& bs, const uint8_t* base);
            void Execute(const ScriptContext& sc);

			inline std::string GetName()
			{
				return m_name;
			}
        private:
            std::string m_name;
            Flags m_flags;
            uint32_t m_size;
            uint16_t m_argCount;
			uint8_t m_regCount;
	};
}