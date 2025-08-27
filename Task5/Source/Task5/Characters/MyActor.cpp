#include "MyActor.h"
#include "../LogTests.h"

AMyActor::AMyActor()
{
	PrimaryActorTick.bCanEverTick = true;
	Coords.Reserve(11);
	Coords.Add(FVector2D(0, 0));
	Idx = 0;
}

void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 1; i < 11; i++)
	{
		FVector2d next = Coords[i - 1];
		next.X += Step();
		next.Y += Step();
		Coords.Add(next);
	}

	// ToString : FString 반환
	// UE_LOG에서 Const TCHAR* 를 요구하기에 
	// Operator* 연산자를 이용하여 해당 타입으로 변환
	UE_LOG(LogCoord, Log, TEXT("현재 위치 : %s"),*Coords[Idx].ToString());
}

int32_t AMyActor::Step()
{
	return FMath::RandRange(0, 1);
}

void AMyActor::Move()
{
	if (Idx >= Coords.Num() - 1)
		return;

	Idx++;
	UE_LOG(LogCoord, Log, TEXT("현재 위치 : %s"), *Coords[Idx].ToString());
}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
}

