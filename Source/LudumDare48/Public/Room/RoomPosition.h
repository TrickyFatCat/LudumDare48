#pragma once

#include "CoreMinimal.h"

struct FRoomPosition
{

	FRoomPosition(): PositionX(0), PositionY(0) {}
	FRoomPosition(const int X, const int Y)
	{
		PositionX = X;
		PositionY = Y;
	};

	int PositionX;

	int PositionY;
};