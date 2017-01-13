#include "engine.hpp"
#include "stack.hpp"
#include "bytecode.hpp"
#include "../displayobject.hpp"
#include "../util.hpp"
#include <iostream>
using namespace libapt;
using namespace libapt::as;

inline void Align(uint8_t *&ptr)
{
	ptr = reinterpret_cast<uint8_t *>(4 * ((reinterpret_cast<uintptr_t>(ptr) + 3) / 4));
}

void Engine::Execute(std::shared_ptr<Object> scope, const uint8_t* bc, std::shared_ptr<Apt> owner)
{
	uint8_t* bs = const_cast<uint8_t*>(bc);
	//create the execution context
	Context context;
	context.Create(scope, owner);


	while (!Opcode(context, bs))
	{
	}
}

void Engine::Execute(Context& context, Function& f, std::vector<Value>& args, std::shared_ptr<Apt> owner)
{
	uint8_t* bs = f.Code;
	//create the execution context
	context.ResizeRegisters(f.nRegisters);
    if(f.Name=="ShowMainMenu")
        int a=0;
	for (int i=0;i<f.nParams;++i)
	{
		Value p;
		if (i < args.size())
			p = args[i];
		else
			p.FromString("");
		context.SetParameter(f.Params[i], p);
	}

	uint32_t location = 0;
	while (location<f.Size && !Opcode(context, bs))
	{
		location = (bs - f.Code);
	}
	context.ClearParams();
}

bool Engine::Opcode(Context& c, uint8_t*& bs)
{
	auto& s = c.GetStack();
	auto& cp = c.GetConstants();
	auto& owner = c.GetOwner();
	uint32_t num = 0;
	std::string str;
	std::shared_ptr<Object> obj;
	Value v;
	Action a = static_cast<Action>(read<uint8_t>(bs));
	bool align = RequireAlign(a);
	
	if(align)
		Align(bs);

	switch (a)
	{
	case NEXTFRAME:
		c.GetScope()->NextFrame();
		break;
	case PLAY:
		c.GetScope()->SetPlaystate(Object::PLAYING);
		break;
	case STOP:
		c.GetScope()->SetPlaystate(Object::STOPPED);
		break;
    case GETURL:
        GetUrl(c,bs);
        break;
	case ADD:
		Add(c);
		break;
	case LOGICALNOT:
		LogicalNot(c);
		break;
	case POP:
		s.Pop();
		break;
	case SETVARIABLE:
		SetVariable(c);
		break;
	case STRINGCONCAT:
		StringConcat(c);
		break;
	case SETPROPERTY:
		SetProperty(c);
		break;
	case TRACE:
		v = s.Pop();
		std::cout << "TRACE: " << v.ToString() << std::endl;
		break;
	case NEWADD:
		NewAdd(c);
		break;
	case NEWEQUALS:
		NewEquals(c);
		break;
	case DUP:
		v = s.Pop();
		s.Push(v);
		s.Push(v);
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
	case EA_PUSHTHIS:
		v.FromObject(c.GetScope());
		s.Push(v);
		break;
	case EA_PUSHGLOBAL:
		v.FromObject(c.GetGlobal());
		s.Push(v);
		break;
	case EA_PUSHTRUE:
		v.FromBoolean(true);
		s.Push(v);
		break;
	case EA_PUSHFALSE:
		v.FromBoolean(false);
		s.Push(v);
		break;
	case EA_PUSHUNDEFINED:
		s.Push(v);
		break;
	case GOTOFRAME:
		num = read<uint32_t>(bs);
		c.GetScope()->SetCurrentFrame(num);
		break;
	case SETREGISTER:
		SetRegister(c,bs );
		break;
	case CONSTANTPOOL:
		Constantpool(c,bs);
		break;
	case DEFINEFUNCTION2:
		DefineFunction2(c, bs);
		break;
	case PUSHDATA:
		PushData(c, bs);
		break;
	case GETURL2:
		GetUrl2(c);
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
		v = GetConstant(c, read<uint8_t>(bs));
		s.Push(v);
		break;
	case EA_GETSTRINGVAR:
		GetStringVar(c, bs);
		break;
	case EA_PUSHVALUEOFVAR:
		PushValue(c, bs);
		break;
	case EA_GETNAMEDMEMBER:
		GetNamedMember(c, bs);
		break;
	case EA_GETSTRINGMEMBER:
		str = readString(c.GetOwner()->GetBase() + read<uint32_t>(bs));
		break;
	case EA_CALLNAMEDFUNCTIONPOP:
		CallNamedFunctionPop(c, bs);
		break;
	case EA_CALLNAMEDMETHODPOP:
		CallNamedMethodPop(c, bs);
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
	Value member = s.Pop();
	Value object = s.Pop();
	object.ToObject()->SetProperty(member.ToString(), value);
}

void Engine::SetVariable(Context& c)
{
	auto& s = c.GetStack();
	Value value = s.Pop();
	Value name = s.Pop();
	c.SetVariable(name.ToString(), value);
}

void Engine::SetProperty(Context& c)
{
	auto& s = c.GetStack();
	Value value = s.Pop();
	Value varname = s.Pop();
	c.GetScope()->SetProperty(varname.ToString(), value);
}

void Engine::SetRegister(Context& c, uint8_t*& bs)
{
	auto& s = c.GetStack();
	Value value = s.Pop();
	uint32_t reg = read<uint32_t>(bs);
	c.SetRegister(reg, value);
}

Value Engine::GetConstant(Context& c,const uint8_t num)
{
	auto& cp = c.GetConstants();
	if (cp.size() > num)
		return cp[num];

	return Value();
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

void Engine::NewAdd(Context& c)
{
	auto& s = c.GetStack();
	Value a = s.Pop();
	Value b = s.Pop();
	Value result;

	if (a.GetType() == Value::STRING &&
		b.GetType() == Value::STRING)
	{
		std::string concStr = b.ToString() + a.ToString();
		result.FromString(concStr);
	}
	else
	{
		assert(0);
	}

	s.Push(result);
}

void Engine::StringConcat(Context & c)
{
	auto& s = c.GetStack();
	Value a = s.Pop();
	Value b = s.Pop();
	Value result;
	std::string concStr = b.ToString() + a.ToString();
	result.FromString(concStr);
	s.Push(result);
}

void Engine::LogicalNot(Context& c)
{
	auto& s = c.GetStack();
	Value v = s.Pop();
	bool b = v.ToBoolean();
	Value result;
	result.FromBoolean(!b);
	s.Push(result);
}

void Engine::NewEquals(Context& c)
{
	auto& s = c.GetStack();
	Value a = s.Pop();
	Value b = s.Pop();
	Value result;
	result.FromBoolean(a == b);
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

void Engine::DefineFunction(Context& c, uint8_t*& bs)
{
	auto owner = c.GetOwner();
	Function f;
	f.nRegisters = 4;
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
	
	Value v;
	v.FromFunction(f);
	//member of current scope
	if (f.Name.size() > 0)
		c.GetScope()->SetVariable(f.Name, v);
	//anonymous function, pushed onto the stack
	else
		c.GetStack().Push(v);
}

void Engine::DefineFunction2(Context& c, uint8_t*& bs)
{
	auto owner = c.GetOwner();
	Function f;
	uint32_t strOffset = read<uint32_t>(bs);
	if (strOffset)
		f.Name = readString(owner->GetBase() + strOffset);

	f.nParams = read<uint32_t>(bs);
	f.nRegisters = 256;

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
	Value v;
	v.FromFunction(f);
	//member of current scope
	if (f.Name.size() > 0)
		c.GetScope()->SetVariable(f.Name, v);
	//anonymous function, pushed onto the stack
	else
		c.GetStack().Push(v);
}

void Engine::CallNamedFunctionPop(Context& c, uint8_t *& bs)
{
	auto& s = c.GetStack();
	std::string func;
	uint32_t argCount;
	Value v;
	v = GetConstant(c, read<uint8_t>(bs));
	func = v.ToString();
	v = s.Pop();
	argCount = v.ToInteger();
	std::vector<Value> args;
	for (int i = 0; i < argCount; ++i)
	{
		args.push_back(s.Pop());
	}
	v = c.GetScope()->GetVariable(func);
	if (v.GetType() == Value::UNDEFINED)
		assert(0);

	Function f = v.ToFunction();
	Execute(c, f, args, c.GetOwner());
}

void Engine::GetUrl(Context& c,uint8_t*& bs)
{
    auto& s = c.GetStack();
    auto& owner = c.GetOwner();
    std::string s1 = readString(owner->GetBase()+read<uint32_t>(bs));
    std::string s2 = readString(owner->GetBase()+read<uint32_t>(bs));
    //TODO call some engine function
}

void Engine::GetUrl2(Context & c)
{
	auto& s = c.GetStack();
	Value target = s.Pop();
	Value url = s.Pop();
	//TODO call some engine function
}

void Engine::CallNamedMethodPop(Context& c, uint8_t *& bs)
{
	auto& s = c.GetStack();
	std::string func;
	std::shared_ptr<Object> obj;
	uint32_t argCount;
	Value v;
	v = GetConstant(c, read<uint8_t>(bs));
	func = v.ToString();
	v = c.GetStack().Pop();
	obj = v.ToObject();
	v = s.Pop();
	argCount = v.ToInteger();
	std::vector<Value> args;
	for (int i = 0; i < argCount;++i)
	{
		args.push_back(s.Pop());
	}

    if(obj==nullptr)
        assert(0);

	if (func == "stop")
	{
		obj->SetPlaystate(Object::STOPPED);
	}
    else if(func=="gotoAndPlay")
    {
		uint32_t frame = obj->GetProperty(args[0].ToString()).ToInteger();
        obj->SetCurrentFrame(frame);
        obj->SetPlaystate(Object::PLAYING);
    }
	else
	{
		v = obj->GetProperty(func);
		if (v.GetType() == Value::UNDEFINED)
			assert(0);

		Function f = v.ToFunction();
		Execute(c, f, args, c.GetOwner());
	}
}

void Engine::GetNamedMember(Context & c, uint8_t *& bs)
{
	Value v;
	auto& s = c.GetStack();
	v = s.Pop();
	auto obj = v.ToObject();
	v = GetConstant(c, read<uint8_t>(bs));
	s.Push(obj->GetProperty(v.ToString()));
}

void Engine::GetStringVar(Context& c, uint8_t*& bs)
{
	Value v;
	std::shared_ptr<Object> obj = nullptr;

	std::string str = readString(c.GetOwner()->GetBase() + read<uint32_t>(bs));
	// get root object
	if (str == "_root")
		obj = GetRoot(c);
	// get parent object
	else if (str == "_parent")
		obj = GetParent(c);
	else if (str == "extern")
		obj = c.GetExtern();
	//get another object
	else
	{
		auto current = std::dynamic_pointer_cast<DisplayObject>(c.GetScope());
		obj = current->GetProperty(str).ToObject();

	}

	v.FromObject(obj);
	c.GetStack().Push(v);
}

void Engine::PushValue(Context& c, uint8_t*& bs)
{
	Value v;
	auto& cp = c.GetConstants();
	std::shared_ptr<Object> obj = nullptr;

	std::string str = cp[read<uint8_t>(bs)].ToString();
	if (c.CheckParam(str))
	{
		v = c.GetParameter(str);
	}
	// get root object
	else if (str == "_root")
	{
		obj = GetRoot(c);
		v.FromObject(obj);
	}
	// get parent object
	else if (str == "_parent")
	{
		obj = GetParent(c);
		v.FromObject(obj);
	}
	else if (str == "extern")
	{
		obj = c.GetExtern();
		v.FromObject(obj);
	}
	//get another object
	else
	{
		auto current = std::dynamic_pointer_cast<DisplayObject>(c.GetScope());
		obj = current->GetProperty(str).ToObject();
		v.FromObject(obj);
	}

	c.GetStack().Push(v);
}

void Engine::PushData(Context & c, uint8_t *& bs)
{
	Value v;
	auto& s = c.GetStack();
	auto& owner = c.GetOwner();
	uint32_t count = read<uint32_t>(bs);
	uint8_t* constants = const_cast<uint8_t*>(owner->GetBase()) + read<uint32_t>(bs);
	
	//push constants to the stack
	for (int i = 0; i < count; ++i)
	{
		uint32_t index = read<uint32_t>(constants);
		auto entry = owner->GetConstant(index);
		v.FromConstant(entry);
		v = c.GetRegister(v.ToInteger());
		s.Push(v);
	}
}

std::shared_ptr<Object> Engine::GetRoot(Context& c)
{
	std::shared_ptr<Object> root = nullptr;
	auto current = std::dynamic_pointer_cast<DisplayObject>(c.GetScope());
	while (current->GetParent() != nullptr)
	{
		current = current->GetParent();
	}

	root = current;
	return root;
}

std::shared_ptr<Object> Engine::GetParent(Context& c)
{
	auto current = std::dynamic_pointer_cast<DisplayObject>(c.GetScope());
	return current->GetParent();
}