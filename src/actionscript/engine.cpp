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
	uint8_t* bs = const_cast<uint8_t*>(bc);
	//create the execution context
	Context context;
	context.Create(scope, owner);


	while (!Opcode(context, bs))
	{
	}
}


bool Engine::Opcode(Context& c, uint8_t*& bs)
{
	auto& s = c.GetStack();
	auto& cp = c.GetConstants();
	auto& owner = c.GetOwner();
	uint32_t num = 0;
	std::string str;
	Value v;
	Action a = static_cast<Action>(read<uint8_t>(bs));
	bool align = RequireAlign(a);
	
	if(align)
		Align(bs);

	switch (a)
	{
	case STOP:
		c.GetScope()->SetPlaystate(Object::STOPPED);
		break;
	case ADD:
		Add(c);
		break;
	case LOGICALNOT:
		LogicalNot(c);
		break;
	case SETVARIABLE:
		SetVariable(c);
		break;
	case SETPROPERTY:
		SetProperty(c);
		break;
	case SETMEMBER:
		SetMember(c);
		break;
	case EA_PUSHZERO:
		v.FromByte(0);
		s.Push(v);
		break;
	case EA_PUSHONE:
		v.FromByte(1);
		s.Push(v);
		break;
	case EA_PUSHFALSE:
		v.FromBoolean(false);
		s.Push(v);
		break;
	case GOTOFRAME:
		num = read<uint32_t>(bs);
		c.GetScope()->SetCurrentFrame(num);
		break;
	case CONSTANTPOOL:
		Constantpool(c,bs);
		break;
	case DEFINEFUNCTION2:
		DefineFunction2(c, bs);
		break;
	case DEFINEFUNCTION:
		DefineFunction(c, bs);
		break;
	case BRANCHIFTRUE:
		BranchIfTrue(c, bs);
		break;
	case EA_PUSHSTRING:
		str = readString(owner->GetBase() + read<uint32_t>(bs));
		v.FromString(str);
		s.Push(v);
		break;
	case EA_PUSHCONSTANT:
 		num = read<uint8_t>(bs);
		v = cp[num];
		s.Push(v);
		break;
	case EA_GETSTRINGVAR:
		break;
	case EA_PUSHVALUEOFVAR:
		v.FromByte(read<uint8_t>(bs));
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

void Engine::SetMember(Context& c)
{
	auto& s = c.GetStack();
	Value value = s.Pop();
	Value obj = s.Pop();
	int a = 0;
}

void Engine::SetVariable(Context& c)
{
	auto& s = c.GetStack();
	Value value = s.Pop();
	Value varname = s.Pop();
	int a = 0;
}

void Engine::SetProperty(Context& c)
{
	auto& s = c.GetStack();
	Value value = s.Pop();
	Value varname = s.Pop();
	int a = 0;
}

void Engine::Add(Context& c)
{
	auto& s = c.GetStack();
	Value a = s.Pop();
	Value b = s.Pop();
	float result = a.ToFloat() + b.ToFloat();
	Value r;
	r.FromFloat(result);
	s.Push(r);
}

void Engine::LogicalNot(Context& c)
{
	auto& s = c.GetStack();
	Value v = s.Pop();
	float number = v.ToFloat();
	Value result;
	if (number == 0)
		result.FromBoolean(true);
	else
		result.FromBoolean(false);

	s.Push(result);
}

void Engine::BranchIfTrue(Context& c, uint8_t*& bs)
{
	auto& s = c.GetStack();
	int32_t offset = read<int32_t>(bs);
	Value cond = s.Pop();
	if (cond.ToBoolean() == true)
	{
		bs += offset;
	}
}

void Engine::Constantpool(Context& c, uint8_t*& bs)
{
	auto owner = c.GetOwner();
	auto& cp = c.GetConstants();
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

Function& Engine::DefineFunction(Context& c, uint8_t*& bs)
{
	auto owner = c.GetOwner();
	Function f;
	uint32_t strOffset = read<uint32_t>(bs);
	if (strOffset)
		f.Name = readString(owner->GetBase() + strOffset);

	f.nParams = read<uint32_t>(bs);
	uint32_t pOffset = read<uint32_t>(bs);
	if (pOffset)
	{
		uint8_t* params = const_cast<uint8_t*>(owner->GetBase()) + pOffset;
		for (uint32_t i = 0; i < f.nParams; i++)
		{
			auto param = readString(owner->GetBase() + read<uint32_t>(params));
			f.Params.push_back(param);
		}
	}
	f.Size = read<uint32_t>(bs);
	bs += 8;
	f.Code = bs;	
	bs += f.Size;
	return f;
}

Function& Engine::DefineFunction2(Context& c, uint8_t*& bs)
{
	auto owner = c.GetOwner();
	Function f;
	uint32_t strOffset = read<uint32_t>(bs);
	if (strOffset)
		f.Name = readString(owner->GetBase() + strOffset);

	f.nParams = read<uint32_t>(bs);
	//regCount
	read<uint8_t>(bs);
	//flags
	read<uint16_t>(bs);
	//skip 1
	bs += 1;

	uint32_t pOffset = read<uint32_t>(bs);
	f.Size = read<uint32_t>(bs);

	if (pOffset)
	{
		uint8_t* params = const_cast<uint8_t*>(owner->GetBase()) + pOffset;
		
		for (uint32_t i = 0; i < f.nParams; i++)
		{
			Argument p;
			p.Register = read<uint32_t>(params);
			p.Name = readString(owner->GetBase() + read<uint32_t>(params));
		}
	}

	bs += 8;
	bs += f.Size;
	return f;
}