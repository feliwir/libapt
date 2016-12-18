#pragma once
#include <vector>
#include "container.hpp"

namespace libapt
{
	class Sprite : public Container
	{
	public:
		virtual void Parse(uint8_t*& iter);
		void Init();
		void SetInitaction(uint8_t* bc);
	private:
		uint8_t* m_bc;
	};
}