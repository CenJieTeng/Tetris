#include "stdafx.h"

Block_I::Block_I() : State(1)
{
	Shape[0] = { 4, 0 };
	Shape[1] = { 5, 0 };
	Shape[2] = { 6, 0 };
	Shape[3] = { 7, 0 };
}

Block_I::~Block_I()
{
}

bool Block_I::ChangeBlock(int map[Width][Height])
{
	int tempState = State;
	POINT tempShape[4];
	for (int i = 0; i < 4; i++)
	{
		tempShape[i].x = Shape[i].x;
		tempShape[i].y = Shape[i].y;
	}

	if (++State > 2)
		State = 1;

	switch (State)
	{
	case 1:
		Shape[0] = { Shape[0].x - 1, Shape[0].y + 1 };
		Shape[2] = { Shape[2].x + 1, Shape[2].y - 1 };
		Shape[3] = { Shape[3].x + 2, Shape[3].y - 2 };
		break;
	case 2:
		Shape[0] = { Shape[0].x + 1, Shape[0].y - 1 };
		Shape[2] = { Shape[2].x - 1, Shape[2].y + 1 };
		Shape[3] = { Shape[3].x - 2, Shape[3].y + 2 };
		break;
	}

	if (!CheckChangeBlock(map))
	{
		State = tempState;
		for (int i = 0; i < 4; i++)
		{
			Shape[i].x = tempShape[i].x;
			Shape[i].y = tempShape[i].y;
		}

		return false;
	}

	return true;
}