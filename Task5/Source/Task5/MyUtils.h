#pragma once

#include "CoreMinimal.h"

namespace Task5::Utils
{
	FORCEINLINE double GetDistance(const FVector& pos1, const FVector& pos2)
	{
		return FMath::Sqrt((pos2.X - pos1.X) * (pos2.X - pos1.X) + (pos2.Y - pos1.Y) * (pos2.Y - pos1.Y));
	}

	FORCEINLINE double GetDistanceSquare(const FVector& pos1, const FVector& pos2)
	{
		return (pos2.X - pos1.X) * (pos2.X - pos1.X) + (pos2.Y - pos1.Y) * (pos2.Y - pos1.Y);
	}

	FORCEINLINE bool IsSuccess50Percent()
	{
		return FMath::RandBool();
	}
}