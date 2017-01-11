#pragma once
#include <vector>
#include "container.hpp"

namespace libapt
{
	class Sprite : public Container
	{
	public:
		virtual void Parse(uint8_t*& iter) override;
		void Init();
		void SetInitaction(uint8_t* bc);
	};
}