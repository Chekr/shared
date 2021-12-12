#ifndef CHIP_2A03_H
#define CHIP_2A03_H


class Bus;

// NES Audiot Processing Unit (apu)
class Chip2a03
{
public:
	Chip2a03();
	~Chip2a03();

	void Reset();
	void Read();
	void Write();
	void Tick();

};


#endif
