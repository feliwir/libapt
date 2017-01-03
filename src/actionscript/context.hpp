#pragma once
#include <libapt/apt.hpp>
#include "stack.hpp"
#include "function.hpp"
#include "object.hpp"
#include <memory>

namespace libapt
{
    namespace as
    {
        class Context
        {
            public:
				Context();
				void Create(Object& scope, std::shared_ptr<Apt> owner);

				inline Stack& GetStack()
				{
					return m_stack;
				}

				inline std::vector<Value>& GetConstants()
				{
					return m_constants;
				}

				inline const std::shared_ptr<Apt> GetOwner()
				{
					return m_owner;
				}

				inline Object* GetScope()
				{
					return m_scope;
				}
            private:
				std::shared_ptr<Apt> m_owner;
				Stack m_stack;
				std::vector<Value> m_constants;
				Object* m_scope;
        };
    }
}