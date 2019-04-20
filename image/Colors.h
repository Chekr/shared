#ifndef _COLOR_H_
#pragma once
#define _COLOR_H_

#pragma pack(push, 1)
struct Rgb
{
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct Rgba
{
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};
#pragma pack(pop)



#endif
