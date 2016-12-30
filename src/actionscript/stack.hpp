#pragma once
#include <stack>
#include "value.hpp"

namespace libapt
{
	namespace as
	{
		class Stack
		{
		public:
			void Push(Value v);
			const Value Pop();
		private:
			std::stack<Value> m_stack;
		};
	}
}