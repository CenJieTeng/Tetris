#include "stdafx.h"

Block_O::Block_O()
{
	Shape[0] = { 5, 0 };
	Shape[1] = { 5, 1 };
	Shape[2] = { 6, 0 };
	Shape[3] = { 6, 1 };
}

Block_O::~Block_O()
{
}

bool Block_O::ChangeBlock(int map[WIDTH][HEIGHT])
{
	return true;
}