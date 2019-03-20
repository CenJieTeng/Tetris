#pragma once
#ifndef BLOCK_I_H
#define BLOCK_I_H
#include "block.h"

class Block_I : public Block
{
public:
	Block_I();
	~Block_I();

private:
	int State;
	virtual bool ChangeBlock(int map[WIDTH][HEIGHT]);
	//virtual bool check(int map[WIDTH][HEIGHT]);
};

#endif // !Block_I_H