#pragma once
#ifndef BLOCK_Z_H
#define BLOCK_Z_H
#include "block.h"

class Block_Z : public Block
{
public:
	Block_Z();
	~Block_Z();

private:
	int State;
	virtual bool ChangeBlock(int map[Width][Height]);
	//virtual bool check(int map[WIDTH][HEIGHT]);
};

#endif // !BLOCK_J_H