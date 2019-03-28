#include "stdafx.h"

Block_T::Block_T() : State(1)
{
	Shape[0] = { 6, 0 };
	Shape[1] = { 5, 1 };
	Shape[2] = { 6, 1 };
	Shape[3] = { 7, 1 };
}

Block_T::~Block_T()
{
}

bool Block_T::ChangeBlock(int map[Width][Height])
{
	int tempState = State;
	POINT tempShape[4];
	for (int i = 0; i < 4; i++)
	{
		tempShape[i].x = Shape[i].x;
		tempShape[i].y = Shape[i].y;
	}

	if (!CheckChangeBlock(map))
		return false;

	if (++State > 4)
		State = 1;

	switch (State)
	{
	case 1:
		Shape[0] = { Shape[0].x - 1, Shape[0].y - 1 };
		Shape[1] = { Shape[1].x - 1, Shape[1].y + 1 };
		//Shape[2] = { Shape[2].x + 0, Shape[2].y - 1 };
		Shape[3] = { Shape[3].x + 1, Shape[3].y - 1 };
		break;
	case 2:
		Shape[0] = { Shape[0].x - 1, Shape[0].y + 1 };
		Shape[1] = { Shape[1].x + 1, Shape[1].y + 1 };
		Shape[3] = { Shape[3].x - 1, Shape[3].y - 1 };
		break;
	case 3:
		Shape[0] = { Shape[0].x + 1, Shape[0].y + 1 };
		Shape[1] = { Shape[1].x + 1, Shape[1].y - 1 };
		//Shape[2] = { Shape[2].x + 0, Shape[2].y - 1 };
		Shape[3] = { Shape[3].x - 1, Shape[3].y + 1 };
		break;
	case 4:
		Shape[0] = { Shape[0].x + 1, Shape[0].y - 1 };
		Shape[1] = { Shape[1].x - 1, Shape[1].y - 1 };
		//Shape[2] = { Shape[2].x + 0, Shape[2].y - 1 };
		Shape[3] = { Shape[3].x + 1, Shape[3].y + 1 };
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