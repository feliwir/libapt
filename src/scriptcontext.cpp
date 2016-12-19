#include <libapt/scriptcontext.hpp>
#include <libapt/apt.hpp>
#include "container.hpp"
#include "util.hpp"
#include <iostream>
using namespace libapt;
using namespace libapt::Bytecode;

inline void Align(uint8_t *&ptr)
{
    ptr = reinterpret_cast<uint8_t *>(4 * ((reinterpret_cast<uintptr_t>(ptr) + 3) / 4));
}

void ScriptContext::Execute(const uint8_t *bc, std::shared_ptr<Character> c)
{
    std::shared_ptr<Container> container = std::dynamic_pointer_cast<Container>(c);
    uint8_t *bs = const_cast<uint8_t *>(bc);

    Instruction op;
    do
    {
	op = (Instruction)(*bs);
	bs++;
	ExecuteOpcode(op, bs, container);
    } while (op != Bytecode::ACTION_END);
}

bool ScriptContext::ExecuteOpcode(Instruction op, uint8_t *&bs, std::shared_ptr<Container> container)
{
    auto apt = container->GetOwner();
    switch (op)
    {
    case ACTION_CONSTANTPOOL:
    {
	Align(bs);
	m_constantpool.clear();
	uint32_t count = read<uint32_t>(bs);
	uint32_t *cpd = reinterpret_cast<uint32_t *>(const_cast<uint8_t *>(apt->GetBase()) + read<uint32_t>(bs));

	for (uint32_t i = 0; i < count; i++)
	{
	    m_constantpool.push_back(apt->GetConstant(cpd[i]));
	}
    }
    break;
    case EA_PUSHCONSTANT:
    case EA_PUSHVALUEOFVAR:
    {
	uint8_t val = read<uint8_t>(bs);
	m_stack.push(m_constantpool[val]);
    }
    break;
	case EA_PUSHZERO:
	{
	    Value val(0);
	    m_stack.push(val);
	}
	break;
    case ACTION_DEFINEFUNCTION2:
    {
	Align(bs);
	Function f;
	f.Parse(bs, apt->GetBase());
	m_functions[f.GetName()] = f;
    }
    break;
    case ACTION_SETMEMBER:
	break;
	//create a new object
	case ACTION_NEW:
	{
		std::string name;
		uint32_t nArgs;
		m_stack.top().Get(name);
		m_stack.pop();
		m_stack.top().Get(nArgs);
		m_stack.pop();
		for(int i=0;i<nArgs;++i)
		{
			Value arg = m_stack.top();
			m_stack.pop();
		}
	}
	break;
    default:
	std::cout << "Unimplemented opcode " << static_cast<uint32_t>(op) << std::endl;
	break;
    }
    return true;
}