#ifndef NES_BUS_H
#define NES_BUS_H

#include "chip_6502.h"
#include "chip_2c02.h"
#include "chip_2a03.h"
#include "nes_cartridge.h"

class NesBus
{
public:
	NesBus();
	~NesBus();

public:

	Chip6502 cpu;
	Chip2c02 ppu;
	Chip2a03 apu;
	std::shared_ptr<Cartridge> cart; // bus breakout for cartridge (ROM+extras)

	uint8_t cpuRam[2048];
	uint8_t controller[2];

public:
	// main bus read/write
	void CpuWrite(uint16_t addr, uint8_t data);
	uint8_t CpuRead(uint16_t addr, bool bReadOnly = false);


public:
	// System I/O
	void InsertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void Reset();
	bool Step(); // single clock tick forward	



};


#endif
