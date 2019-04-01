#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#include <memory>
#include "information.h"

class Block
{
public:
	Block();
	virtual ~Block();

public:
	POINT Shape[4];

public:
	bool MoveDownBlovk(int map[Width][Height]);
	bool MoveLeftBlovk(int map[Width][Height]);
	bool MoveRightBlovk(int map[Width][Height]);
	virtual bool ChangeBlock(int map[Width][Height]) = 0;

protected:
	bool CheckChangeBlock(int map[Width][Height]) const;

private:
	bool CheckDown(int map[Width][Height]) const;
	bool CheckLeft(int map[Width][Height]) const;
	bool CheckRight(int map[Width][Height]) const;
};

#endif // !BLOCK_H