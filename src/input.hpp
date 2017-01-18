#pragma once
#include <stdint.h>
namespace libapt
{
    enum Input : uint8_t
    {
        MOUSEBUTTON0	= 0,
		LEFT			= 1,
		RIGHT			= 2,
		HOME			= 3,
		END				= 4,
		INSERT			= 5,
		DELETE			= 6,
		BACKSPACE		= 8,
		ENTER			= 13,
	};
}