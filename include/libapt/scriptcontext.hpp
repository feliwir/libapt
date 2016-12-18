#pragma once
#include "value.hpp"
#include "bytecode.hpp"
#include "scriptfunction.hpp"
#include <stack>
#include <vector>
#include <memory>
#include <map>

namespace libapt
{
	class Character;
	class Container;

	class ScriptContext
	{
	public:
		void Execute(const uint8_t* bc, std::shared_ptr<Character> c);

	private:
		bool ExecuteOpcode(Bytecode::Instruction op, uint8_t*& bytestream, std::shared_ptr<Container> c);
	private:
		std::vector<Value> m_constantpool;
		std::stack<Value> m_stack;
		std::map<std::string, Function> m_functions;
	};
}