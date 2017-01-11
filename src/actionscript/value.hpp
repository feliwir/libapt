#pragma once
#include <libapt/const.hpp>
#include <memory>
#include "function.hpp"

namespace libapt
{
	namespace as
	{
		class Object;
		class Value
		{
		public:
			enum Type
			{
				STRING		= 0,
				FLOAT		= 1,
				NUL			= 2,
				UNDEFINED	= 3,
				REGISTER	= 4,
				BOOLEAN		= 5,
				DOUBLE		= 6,
				INTEGER		= 7,
				OBJECT		= 8,
				FUNCTION	= 9,
			};
		public:
			Value();
			void FromFloat(const float n);
			void FromConstant(const Const::Entry& e);
			void FromByte(const uint8_t n);
			void FromBoolean(const bool c);
			void FromString(const std::string& s);
			void FromObject(std::shared_ptr<Object> obj);
			void FromFunction(const Function& f);

			Function& ToFunction();
			float ToFloat();
			bool ToBoolean();
			uint32_t ToInteger();
			std::string ToString();
			std::shared_ptr<Object> ToObject();

			inline const Type GetType() const
			{
				return m_type;
			}

			bool operator==(const Value& a)
			{
				return (m_type == a.GetType());
			}
		private:
			Type m_type;
			bool m_boolean;
			int m_number;
			double m_float;
			std::string m_string;
			std::shared_ptr<Object> m_object;
			Function m_function;
		};
	}
}