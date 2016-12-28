#pragma once
#include <stdint.h>
#include <string>

namespace libapt
{
	class Import
	{
	public:
		void Parse(uint8_t*& iter, const uint8_t* base);

		inline const std::string& GetMovie() const { return m_movie; }
		inline const std::string& GetName()  const { return m_name; }
		inline const uint32_t GetCharacter() const { return m_character; }
	private:
		std::string m_movie;
		std::string m_name;
		uint32_t m_character;
		uint32_t m_pointer;
	};
}