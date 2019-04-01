#include "stdafx.h"

Block::Block()
{
}

Block::~Block()
{
}

bool Block::MoveDownBlovk(int map[Width][Height])
{
	if (!CheckDown(map))
		return false;

	for (int i = 0; i < 4; i++)
	{
		Shape[i].y += 1;
	}

	return true;
}

bool Block::MoveLeftBlovk(int map[Width][Height])
{
	if (!CheckLeft(map))
		return false;

	for (int i = 0; i < 4; i++)
	{
		Shape[i].x -= 1;
	}

	return true;
}

bool Block::MoveRightBlovk(int map[Width][Height])
{
	if (!CheckRight(map))
		return false;

	for (int i = 0; i < 4; i++)
	{
		Shape[i].x += 1;
	}

	return true;
}

bool Block::CheckDown(int map[Width][Height]) const
{
	//到达底部
	if (Shape[0].y + 1 == Height ||
		Shape[1].y + 1 == Height ||
		Shape[2].y + 1 == Height ||
		Shape[3].y + 1 == Height)
		return false;
	//撞到方块
	if (map[Shape[0].x][Shape[0].y + 1] ||
		map[Shape[1].x][Shape[1].y + 1] ||
		map[Shape[2].x][Shape[2].y + 1] ||
		map[Shape[3].x][Shape[3].y + 1])
		return false;

	return true;
}

bool Block::CheckLeft(int map[Width][Height]) const
{
	//到达左边界
	if (Shape[0].x == 0 ||
		Shape[1].x == 0 ||
		Shape[2].x == 0 ||
		Shape[3].x == 0)
 		return false; 
	//撞到方块
	if (map[Shape[0].x - 1][Shape[0].y] ||
		map[Shape[1].x - 1][Shape[1].y] ||
		map[Shape[2].x - 1][Shape[2].y] ||
		map[Shape[3].x - 1][Shape[3].y])
		return false;

	return true;
}

bool Block::CheckRight(int map[Width][Height]) const
{
	//到达右边界
	if (Shape[0].x == Width - 1 ||
		Shape[1].x == Width - 1 ||
		Shape[2].x == Width - 1 ||
		Shape[3].x == Width - 1)
		return false;
	//撞到方块
	if (map[Shape[0].x + 1][Shape[0].y] ||
		map[Shape[1].x + 1][Shape[1].y] ||
		map[Shape[2].x + 1][Shape[2].y] ||
		map[Shape[3].x + 1][Shape[3].y])
		return false;

	return true;
}

bool Block::CheckChangeBlock(int map[Width][Height]) const
{
	//检测是否超过边界
	if (Shape[0].x < 0 || Shape[0].x > Width - 1 ||
		Shape[1].x < 0 || Shape[1].x > Width - 1 ||
		Shape[2].x < 0 || Shape[2].x > Width - 1 ||
		Shape[3].x < 0 || Shape[3].x > Width - 1 ||
		Shape[0].y < 0 || Shape[0].y > Height - 1 ||
		Shape[1].y < 0 || Shape[1].y > Height - 1 ||
		Shape[2].y < 0 || Shape[2].y > Height - 1 ||
		Shape[3].y < 0 || Shape[3].y > Height - 1)
		return false;

	//检测是否重叠
	if (map[Shape[0].x + 1][Shape[0].y] ||
		map[Shape[1].x + 1][Shape[1].y] ||
		map[Shape[2].x + 1][Shape[2].y] ||
		map[Shape[3].x + 1][Shape[3].y])
		return false;

	return true;
}