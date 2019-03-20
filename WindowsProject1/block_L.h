#pragma once
#ifndef BLOCK_L_H
#define BLOCK_L_H
#include "block.h"

class Block_L : public Block
{
public:
	Block_L();
	~Block_L();

private:
	int State;
	virtual bool ChangeBlock(int map[WIDTH][HEIGHT]);
	//virtual bool check(int map[WIDTH][HEIGHT]);
};

#endif // !BLOCK_L_H