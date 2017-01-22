#pragma once
#include <libapt/apt.hpp>
#include "stack.hpp"
#include "function.hpp"
#include "object.hpp"
#include "extern.hpp"
#include <memory>

namespace libapt
{
    namespace as
    {
        class Context
        {
            public:
				Context();
				void Create(std::shared_ptr<Object> scope, std::shared_ptr<Apt> owner);

				inline std::vector<Value>& GetConstants()
				{
					return m_constants;
				}

				Value GetVariable(const std::string& name)
				{
					return m_scope->GetProperty(name);
				}

				void SetVariable(const std::string& name, Value v)
				{
					m_scope->SetProperty(name, v);
				}

				inline Stack& GetStack()
				{
					return m_stack;
				}

				inline const std::shared_ptr<Apt> GetOwner()
				{
					return m_owner;
				}

				inline std::shared_ptr<Object> GetScope()
				{
					return m_scope;
				}

				inline Value GetRegister(uint32_t id)
				{
					return m_registers[id];
				}

				inline void SetRegister(uint32_t id, const Value& v)
				{
					m_registers[id] = v;
				}

				inline std::shared_ptr<Object> GetGlobal()
				{
					return s_global;
				}

				inline void ResizeRegisters(uint32_t size)
				{
					m_registers.resize(size);
				}

				inline std::shared_ptr<Object> GetExtern()
				{
					return m_extern;
				}

				inline bool CheckParam(const std::string& name)
				{
					return (m_params.find(name) != m_params.end());
				}

				inline Value GetParameter(const std::string& name)
				{
					return m_params[name];
				}

				inline void SetParameter(const std::string& name, Value v)
				{
					m_params[name] = v;
				}

				inline void ClearParams()
				{
					m_params.clear();
				}

				inline void SetConstantpool(const std::vector<Value>& cp)
				{
					m_constants = cp;
				}
            private:
				std::shared_ptr<Apt> m_owner;
				Stack m_stack;
				std::vector<Value> m_constants;
				//function registers
				std::vector<Value> m_registers;
				//function parameters
				std::map<std::string, Value> m_params;
				//the local script object
				std::shared_ptr<Object> m_scope;
				//the global script object
				static std::shared_ptr<Object> s_global;
				//the script object used to access extern variables/functions
				std::shared_ptr<Extern> m_extern;
        };
    }
}