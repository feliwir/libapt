#include "engine.hpp"
#include "stack.hpp"
#include "bytecode.hpp"
#include "../util.hpp"
#include <iostream>
using namespace libapt;
using namespace libapt::as;

inline void Align(uint8_t *&ptr)
{
	ptr = reinterpret_cast<uint8_t *>(4 * ((reinterpret_cast<uintptr_t>(ptr) + 3) / 4));
}

void Engine::Execute(Object& scope, const uint8_t* bc, std::shared_ptr<Apt> owner)
{
	Stack s;
	std::vector<Value> cp;
	uint8_t* bs = const_cast<uint8_t*>(bc);

	while (!Opcode(scope, bs, s, cp, owner))
	{
	}
}


bool Engine::Opcode(Object& scope, uint8_t*& bs, Stack& s,
	std::vector<Value>& cp, std::shared_ptr<Apt> owner)
{
	int index = 0;
	Value v;
	Action a = static_cast<Action>(read<uint8_t>(bs));
	bool align = RequireAlign(a);
	
	if(align)
		Align(bs);

	switch (a)
	{
	case STOP:
		scope.SetPlaystate(Object::STOPPED);
		break;
	case ADD:
		Add(s);
		break;
	case LOGICALNOT:
		LogicalNot(s);
		break;
	case SETVARIABLE:
		SetVariable(s);
		break;
	case SETMEMBER:
		SetMember(s);
		break;
	case CONSTANTPOOL:
		Constantpool(bs, cp, owner);
		break;
	case DEFINEFUNCTION:
		DefineFunction(bs, owner);
		break;
	case BRANCHIFTRUE:
		BranchIfTrue(s, bs);
		break;
	case EA_PUSHVALUEOFVAR:
		index = bs - owner->GetBase();
		v.FromByte(read<uint8_t>(bs));
		s.Push(v);
		break;
	case EA_PUSHCONSTANT:
 		index = read<uint8_t>(bs);
		v = cp[index];
		s.Push(v);
		break;
	case EA_GETNAMEDMEMBER:
		break;
	case END:
		return true;
		break;
	default:
		std::cout << "Unimplemented opcode: " << static_cast<int>(a) << std::endl;
		break;
	}
	return false;
}

void Engine::SetMember(Stack& s)
{
	Value value = s.Pop();
	Value obj = s.Pop();
	int a = 0;
}

void Engine::SetVariable(Stack& s)
{
	Value value = s.Pop();
	Value varname = s.Pop();
	int a = 0;
}

void Engine::Add(Stack & s)
{
	Value a = s.Pop();
	Value b = s.Pop();
	float result = a.ToFloat() + b.ToFloat();
	Value r;
	r.FromFloat(result);
	s.Push(r);
}

void Engine::LogicalNot(Stack& s)
{
	Value v = s.Pop();
	float number = v.ToFloat();
	Value result;
	if (number == 0)
		result.FromBoolean(true);
	else
		result.FromBoolean(false);

	s.Push(result);
}

void Engine::BranchIfTrue(Stack& s, uint8_t*& bs)
{
	int32_t offset = read<int32_t>(bs);
	Value cond = s.Pop();
	if (cond.ToBoolean() == true)
	{
		bs += offset;
	}
}

void Engine::Constantpool(uint8_t*& bs, std::vector<Value>& cp, std::shared_ptr<Apt> owner)
{
	cp.clear();
	uint32_t count = read<uint32_t>(bs);
	uint32_t *cpd = reinterpret_cast<uint32_t *>(const_cast<uint8_t *>(owner->GetBase()) + read<uint32_t>(bs));

	for (uint32_t i = 0; i < count; i++)
	{
		Const::Entry c = owner->GetConstant(cpd[i]);
		Value v;
		v.FromConstant(c);
		cp.push_back(v);
	}
}

Function& Engine::DefineFunction(uint8_t*& bs, std::shared_ptr<Apt> owner)
{
	Function f;
	uint32_t strOffset = read<uint32_t>(bs);
	if (strOffset)
		f.name = readString(owner->GetBase() + strOffset);

	f.nParams = read<uint32_t>(bs);
	uint32_t pOffset = read<uint32_t>(bs);
	if (pOffset)
	{
		uint8_t* params = const_cast<uint8_t*>(owner->GetBase()) + pOffset;
		for (uint32_t i = 0; i < f.nParams; i++)
		{
			auto param = readString(owner->GetBase() + read<uint32_t>(params));
			f.params.push_back(param);
		}
	}
	f.size = read<uint32_t>(bs);
	bs += 8;
	f.bc = bs;	
	bs += f.size;
	return f;
}