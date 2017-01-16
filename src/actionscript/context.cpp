#include "context.hpp"
using namespace libapt;
using namespace libapt::as;

std::shared_ptr<Object> Context::s_global = std::make_shared<Object>();

Context::Context() : m_scope(nullptr)
{
}

void Context::Create(std::shared_ptr<Object> scope, std::shared_ptr<Apt> owner)
{
	m_scope = scope;
	m_owner = owner;
	m_constants = scope->GetConstants();
	m_extern = std::make_shared<Extern>();
}
