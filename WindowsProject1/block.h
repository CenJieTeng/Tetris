#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#include <memory>

//”≈ªØµ„3////////////
#define WIDTH  12
#define HEIGHT 15
/////////////////////

class Block
{
public:
	Block();
	virtual ~Block();

public:
	POINT Shape[4];

public:
	bool MoveDownBlovk(int map[WIDTH][HEIGHT]);
	bool MoveLeftBlovk(int map[WIDTH][HEIGHT]);
	bool MoveRightBlovk(int map[WIDTH][HEIGHT]);
	virtual bool ChangeBlock(int map[WIDTH][HEIGHT]) = 0;

protected:
	bool CheckChangeBlock(int map[WIDTH][HEIGHT]);

private:
	bool CheckDown(int map[WIDTH][HEIGHT]);
	bool CheckLeft(int map[WIDTH][HEIGHT]);
	bool CheckRight(int map[WIDTH][HEIGHT]);
};

#endif // !BLOCK_H