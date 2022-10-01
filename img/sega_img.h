#ifndef SEGA_IMG_H
#define SEGA_IMG_H

#include <iostream>
//#include <string>
#include <cstring>
#include <unordered_set>
#include <sstream>
#include <bitset>

#include <unistd.h>

#include "bitmap.h"

enum PaletteType
{
    RGB = 0,
    MEGA_DRIVE,
    MASTER_SYSTEM,
    GAMEGEAR,
    TMS
};


inline std::string FormatColor(uint8_t r, uint8_t g, uint8_t b, PaletteType type)
{
    std::stringstream ss;

    switch(type)
    {
        case MEGA_DRIVE:
            //DC.W    $0000,$0044,$0066,$0088 ; -BGR
            ss << "$0" << std::uppercase << std::hex <<
                (int(b & 0xff) >> 4) <<
                (int(g & 0xff) >> 4) <<
                (int(r & 0xff) >> 4);
            break;
        case MASTER_SYSTEM:
            //DB %00111111 ; --BBGGRR
            ss << "%00" <<
                std::bitset<2>(b >> 6) <<
                std::bitset<2>(g >> 6) <<
                std::bitset<2>(r >> 6);
            break;
        case GAMEGEAR:
            //DB%11111111,%00001111 ; GGGGRRRR,----BBBB
            ss << "%" <<
                std::bitset<4>(g >> 4) <<
                std::bitset<4>(r >> 4) <<
                ",%0000" <<
                std::bitset<4>(b >> 4);

            break;
        case RGB:
        default:
            ss << 
                std::to_string(int(r & 0xff)) << "," <<
                std::to_string(int(g & 0xff)) << "," <<
                std::to_string(int(b & 0xff));
            break;
    }

    return ss.str();
}

#endif
