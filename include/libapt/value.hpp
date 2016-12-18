#pragma once
#include <stdint.h>
#include <string>
#include <type_traits>

namespace libapt
{
    class Value
	{
	public:
		enum Type
		{
			UNDEFINED	= 0,
			STRING		= 1,
			INTEGER		= 2,
			FLOAT		= 3,
			BOOLEAN		= 4,
			BYTE		= 5,
			SHORT		= 6
		};

		inline void Set(bool value)
		{
			m_type = BOOLEAN;
			m_integer = static_cast<int32_t>(value);
		}

		inline void Set(int32_t value)
		{
			m_type = INTEGER;
			m_integer = value;
		}

		inline void Set(uint32_t value)
		{
			m_type = INTEGER;
			m_integer = static_cast<int32_t>(value);
		}

		inline void Set(uint8_t value)
		{
			m_type = BYTE;
			m_integer = value;
		}

		inline void Set(int16_t value)
		{
			m_type = SHORT;
			m_integer = value;
		}

		inline void Set(std::string value)
		{
			m_type = STRING;
			m_string = value;
		}

		inline void Set(float value)
		{
			m_type = FLOAT;
			m_float = value;
		}

		inline void Get(bool& val)
		{
			val = static_cast<bool>(m_integer);
		}

		inline void Get(uint32_t& val)
		{
			val = static_cast<uint32_t>(m_integer);
		}

		inline void Get(uint8_t& val)
		{
			val = static_cast<uint8_t>(m_integer);
		}

		inline void Get(int16_t& val)
		{
			val = static_cast<int16_t>(m_integer);
		}

		inline void Get(float& val)
		{
			val = m_float;
		}

		inline void Get(std::string& val)
		{
			val = m_string;
		}
	private:
		Type m_type;
		int32_t m_integer;
		float m_float;
		std::string m_string;
	};
}