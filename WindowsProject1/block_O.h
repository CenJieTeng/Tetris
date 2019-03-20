#pragma once
#ifndef BLOCK_O_H
#define BLOCK_O_H
#include "block.h"

class Block_O : public Block
{
public:
	Block_O();
	virtual ~Block_O();

private:
	virtual bool ChangeBlock(int map[WIDTH][HEIGHT]);
	//virtual bool check(int map[WIDTH][HEIGHT]);
};

#endif // !BLOCK_H