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
			static void Execute(Context& c, Function& f,std::vector<Value>& args, std::shared_ptr<Apt> owner);
		private:
			static bool Opcode(Context& c, uint8_t*& bs);
			///--Opcodes
			static void Constantpool(Context& c,uint8_t*& bs);
			//Functions			
			static void DefineFunction(Context& c, uint8_t*& bs);
			static void DefineFunction2(Context& c, uint8_t*& bs);
			static void CallNamedMethodPop(Context& c, uint8_t*& bs);
			static void CallNamedFunctionPop(Context& c, uint8_t*& bs);
			//Geturl
			static void GetUrl(Context& c, uint8_t*& bs);
			static void GetUrl2(Context& c);
			//Push
			static void PushValue(Context& c, uint8_t*& bs);
			static void PushData(Context& c, uint8_t*& bs);
			//Setters
			static void SetMember(Context& c);
			static void SetVariable(Context& c);
			static void SetProperty(Context& c);
			static void SetRegister(Context& c,uint8_t*& bs);
			//Getters
			static void GetNamedMember(Context& c, uint8_t*& bs);
			static void GetStringVar(Context& c,uint8_t*& bs);
			static Value GetConstant(Context& c, const uint8_t num);
			//Arithmetic functions
			static void Add(Context& c);
			//String operations
			static void NewAdd(Context& c);
			static void StringConcat(Context& c);
			//Logical operators
			static void LogicalNot(Context& c);
			static void NewEquals(Context& c);
			//Branching
			static void BranchIfTrue(Context& c, uint8_t*& bs);
			//Helper functions
			static std::shared_ptr<Object> GetRoot(Context& c);
			static std::shared_ptr<Object> GetParent(Context& c);
		public:
			static as::Engine s_engine;
		};
	}
}