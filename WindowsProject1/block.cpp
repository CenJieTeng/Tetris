#include "stdafx.h"

Block::Block()
{
}

Block::~Block()
{
}

bool Block::MoveDownBlovk(int map[WIDTH][HEIGHT])
{
	if (!CheckDown(map))
		return false;

	for (int i = 0; i < 4; i++)
	{
		Shape[i].y += 1;
	}

	return true;
}

bool Block::MoveLeftBlovk(int map[WIDTH][HEIGHT])
{
	if (!CheckLeft(map))
		return false;

	for (int i = 0; i < 4; i++)
	{
		Shape[i].x -= 1;
	}

	return true;
}

bool Block::MoveRightBlovk(int map[WIDTH][HEIGHT])
{
	if (!CheckRight(map))
		return false;

	for (int i = 0; i < 4; i++)
	{
		Shape[i].x += 1;
	}

	return true;
}

bool Block::CheckDown(int map[WIDTH][HEIGHT])
{
	//����ײ�
	if (Shape[0].y + 1 == HEIGHT ||
		Shape[1].y + 1 == HEIGHT ||
		Shape[2].y + 1 == HEIGHT ||
		Shape[3].y + 1 == HEIGHT)
		return false;
	//ײ������
	if (map[Shape[0].x][Shape[0].y + 1] ||
		map[Shape[1].x][Shape[1].y + 1] ||
		map[Shape[2].x][Shape[2].y + 1] ||
		map[Shape[3].x][Shape[3].y + 1])
		return false;

	return true;
}

bool Block::CheckLeft(int map[WIDTH][HEIGHT])
{
	//������߽�
	if (Shape[0].x == 0 ||
		Shape[1].x == 0 ||
		Shape[2].x == 0 ||
		Shape[3].x == 0)
 		return false; 
	//ײ������
	if (map[Shape[0].x - 1][Shape[0].y] ||
		map[Shape[1].x - 1][Shape[1].y] ||
		map[Shape[2].x - 1][Shape[2].y] ||
		map[Shape[3].x - 1][Shape[3].y])
		return false;

	return true;
}

bool Block::CheckRight(int map[WIDTH][HEIGHT])
{
	//�����ұ߽�
	if (Shape[0].x == WIDTH - 1 ||
		Shape[1].x == WIDTH - 1 ||
		Shape[2].x == WIDTH - 1 ||
		Shape[3].x == WIDTH - 1)
		return false;
	//ײ������
	if (map[Shape[0].x + 1][Shape[0].y] ||
		map[Shape[1].x + 1][Shape[1].y] ||
		map[Shape[2].x + 1][Shape[2].y] ||
		map[Shape[3].x + 1][Shape[3].y])
		return false;

	return true;
}

bool Block::CheckChangeBlock(int map[WIDTH][HEIGHT])
{
	//����Ƿ񳬹��߽�
	if (Shape[0].x < 0 || Shape[0].x > WIDTH - 1 ||
		Shape[1].x < 0 || Shape[1].x > WIDTH - 1 ||
		Shape[2].x < 0 || Shape[2].x > WIDTH - 1 ||
		Shape[3].x < 0 || Shape[3].x > WIDTH - 1 ||
		Shape[0].y < 0 || Shape[0].y > HEIGHT - 1 ||
		Shape[1].y < 0 || Shape[1].y > HEIGHT - 1 ||
		Shape[2].y < 0 || Shape[2].y > HEIGHT - 1 ||
		Shape[3].y < 0 || Shape[3].y > HEIGHT - 1)
		return false;

	//����Ƿ��ص�
	if (map[Shape[0].x + 1][Shape[0].y] ||
		map[Shape[1].x + 1][Shape[1].y] ||
		map[Shape[2].x + 1][Shape[2].y] ||
		map[Shape[3].x + 1][Shape[3].y])
		return false;

	return true;
}