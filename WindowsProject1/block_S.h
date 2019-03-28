#pragma once
#ifndef BLOCK_S_H
#define BLOCK_S_H
#include "block.h"

class Block_S : public Block
{
public:
	Block_S();
	~Block_S();

private:
	int State;
	virtual bool ChangeBlock(int map[Width][Height]);
	//virtual bool check(int map[WIDTH][HEIGHT]);
};

#endif // !BLOCK_S_H