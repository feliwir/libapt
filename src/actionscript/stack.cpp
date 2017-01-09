#include "stack.hpp"
using namespace libapt;
using namespace libapt::as;

void Stack::Push(Value v)
{
	m_stack.push(v);
}

const Value Stack::Pop()
{
	if (m_stack.size() == 0)
		return Value();

	const auto v = m_stack.top();
	m_stack.pop();
	return v;
}
