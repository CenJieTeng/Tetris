#pragma once
#ifndef BLOCK_T_H
#define BLOCK_T_H
#include "block.h"

class Block_T : public Block
{
public:
	Block_T();
	~Block_T();

private:
	int State;
	virtual bool ChangeBlock(int map[Width][Height]);
	//virtual bool check(int map[WIDTH][HEIGHT]);
};

#endif // !Block_T_H