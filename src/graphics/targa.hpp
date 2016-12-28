#pragma once
#include <stdint.h>

namespace libapt
{
    struct Targa
    {
        uint8_t Header[12];
		uint16_t Width;
		uint16_t Height;
        uint8_t Bpp;
        uint8_t Id;      
        uint8_t* Data;
    };
}