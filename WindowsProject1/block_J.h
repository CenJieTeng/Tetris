#pragma once
#ifndef BLOCK_J_H
#define BLOCK_J_H
#include "block.h"

class Block_J : public Block
{
public:
	Block_J();
	~Block_J();

private:
	int State;
	virtual bool ChangeBlock(int map[Width][Height]);
	//virtual bool check(int map[WIDTH][HEIGHT]);
};

#endif // !BLOCK_J_H