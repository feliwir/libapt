#pragma once
#include "object.hpp"

namespace libapt
{
	namespace as
	{
		class Extern : public Object
		{
			virtual Value GetProperty(const std::string& property) override;
			virtual void SetProperty(const std::string& property, Value v) override;
		};
	}
}