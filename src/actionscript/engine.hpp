#pragma once
#include "context.hpp"
#include <stdint.h>


namespace libapt
{
	namespace as
	{
		class Engine
		{
		public:
			//Execute bytecode on the current scope
			static void Execute(std::shared_ptr<Object> scope, const uint8_t* bc,std::shared_ptr<Apt> owner);
		private:
			static bool Opcode(Context& c, uint8_t*& bs);
			///--Opcodes
			static void Constantpool(Context& c,uint8_t*& bs);
			//Functions			
			static const Function DefineFunction(Context& c, uint8_t*& bs);
			static const Function DefineFunction2(Context& c, uint8_t*& bs);
			//Setters
			static void SetMember(Context& c);
			static void SetVariable(Context& c);
			static void SetProperty(Context& c);
			//Getters
			static void GetNamedObject(Context& c,uint8_t*& bs);
			static Value GetConstant(Context& c, const uint8_t num);
			//Arithmetic functions
			static void Add(Context& c);
			//Logical operators
			static void LogicalNot(Context& c);
			static void NewEquals(Context& c);
			//Branching
			static void BranchIfTrue(Context& c, uint8_t*& bs);
		};
	}
}