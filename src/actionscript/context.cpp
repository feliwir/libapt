#include "context.hpp"
using namespace libapt;
using namespace libapt::as;

Context::Context() : m_scope(nullptr)
{
}

void Context::Create(Object& scope, std::shared_ptr<Apt> owner)
{
	m_scope = &scope;
	m_owner = owner;
}
