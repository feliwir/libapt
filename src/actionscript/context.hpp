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

				Value GetVariable(const std::string& name)
				{
					return m_variables[name];
				}

				void SetVariable(const std::string& name, Value v)
				{
					m_variables[name] = v;
				}

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

				inline void AddFunction(const std::string& name, const Function& f)
				{
					m_functions[name] = f;
				}

				inline const Function GetFunction(const std::string& name)
				{
					return m_functions[name];
				}

            private:
				std::shared_ptr<Apt> m_owner;
				Stack m_stack;
				std::vector<Value> m_constants;
				std::map<std::string, Value> m_variables;
				std::map<std::string, Function> m_functions;
				Object* m_scope;
        };
    }
}