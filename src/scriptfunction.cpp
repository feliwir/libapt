#include <libapt/scriptfunction.hpp>
#include <libapt/scriptcontext.hpp>
#include "util.hpp"

using namespace libapt;

void Function::Parse(uint8_t*& bs, const uint8_t* base)
{
	uint32_t strOffset = read<uint32_t>(bs);
	if (strOffset)
		m_name = readString(base + strOffset);

	m_argCount = read<uint32_t>(bs);
	m_regCount = read<uint8_t>(bs);
	uint16_t flags = read<uint16_t>(bs);
	m_flags = *reinterpret_cast<Flags*>(&flags);

	bs += 1;
	uint32_t argsOffset = read<uint32_t>(bs);
	m_size = read<uint32_t>(bs);

	if (argsOffset)
	{
		uint8_t* args = const_cast<uint8_t*>(base + argsOffset);
		for (uint32_t i = 0; i < m_argCount; i++)
		{
			Argument arg;
			arg.Register = read<uint32_t>(args);
			arg.Name = readString(base + read<uint32_t>(args));
		}
	}

	bs += 8;
	bs += m_size;
}


void Function::Execute(const ScriptContext& sc)
{

}
