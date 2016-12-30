#pragma once
#include "object.hpp"
#include "stack.hpp"
#include "function.hpp"
#include <libapt/apt.hpp>
#include <stdint.h>
#include <memory>

namespace libapt
{
	namespace as
	{
		class Engine
		{
		public:
			//Execute bytecode on the current scope
			static void Execute(Object& scope, const uint8_t* bc,std::shared_ptr<Apt> owner);
		private:
			static bool Opcode(Object& scope, uint8_t*& bs, Stack& s, std::vector<Value>& cp, std::shared_ptr<Apt> owner);
			///--Opcodes
			static void Constantpool(uint8_t*& bs, std::vector<Value>& cp,std::shared_ptr<Apt> owner);
			//Functions
			static Function& DefineFunction(uint8_t*& bs, std::shared_ptr<Apt> owner);
			//Setters
			static void SetMember(Stack& s);
			static void SetVariable(Stack& s);
			//Arithmetic functions
			static void Add(Stack& s);
			//Logical operators
			static void LogicalNot(Stack& s);
			//Branching
			static void BranchIfTrue(Stack& s, uint8_t*& bs);
		};
	}
}