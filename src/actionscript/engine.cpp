#include "engine.hpp"
#include "stack.hpp"
#include "bytecode.hpp"
#include "../util.hpp"
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
	Action a = static_cast<Action>(read<uint8_t>(bs));
	bool align = RequireAlign(a);
	Align(bs);

	switch (a)
	{
	case DEFINEFUNCTION:
		DefineFunction(bs, owner);
		break;
	case CONSTANTPOOL:
		Constantpool(bs, cp,owner);
		break;
	case END:
		return true;
		break;
	}
	return false;
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
	f.bc = bs;
	return f;
}