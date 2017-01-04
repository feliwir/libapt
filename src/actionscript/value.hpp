#pragma once
#include <libapt/const.hpp>

namespace libapt
{
	namespace as
	{
		class Value
		{
		public:
			enum Type
			{
				STRING = 0,
				FLOAT = 1,
				NUL = 2,
				UNDEFINED = 3,
				REGISTER = 4,
				BOOLEAN = 5,
				DOUBLE = 6,
				INTEGER = 7,
				CONSTANT = 8
			};
		public:
			Value();
			void FromFloat(const float n);
			void FromConstant(const Const::Entry& e);
			void FromByte(const uint8_t n);
			void FromBoolean(const bool c);
			void FromString(const std::string& s);

			float ToFloat();
			bool ToBoolean();
			std::string ToString();
		private:
			Type m_type;
			bool m_boolean;
			int m_number;
			double m_float;
			std::string m_string;
		};
	}
}