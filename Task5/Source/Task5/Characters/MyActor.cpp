#include "MyActor.h"
#include "../LogTests.h"
#include "../MyUtils.h"

AMyActor::AMyActor()
	:Idx(0),
	IsArrived(false),
	EventCounts(0),
	FullDistance(0.0)
{
	PrimaryActorTick.bCanEverTick = true;
	Coords.Reserve(11);
	Coords.Add(FVector(0, 0, 0));
}

void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	InitTargetCoords();

	// ToString : FString 반환
	// UE_LOG에서 Const TCHAR* 를 요구하기에 
	// Operator* 연산자를 이용하여 해당 타입으로 변환
	UE_LOG(LogCoord, Log, TEXT("현재 위치 : %s"), *Coords[Idx].ToString());
}

int32_t AMyActor::Step()
{
	return FMath::RandRange(0, 1);
}

void AMyActor::Move()
{
	if (Idx >= Coords.Num() - 1)
	{
		IsArrived = true;
		UE_LOG(LogCoord, Log, TEXT("총 발생 이벤트 횟수 : % d"), EventCounts);
		UE_LOG(LogCoord, Log, TEXT("총 이동 거리 : %f"), FullDistance);
		return;
	}

	EventCheck();

	Idx++;

	double distance = Task5::Utils::GetDistance(Coords[Idx], Coords[Idx - 1]);

	UE_LOG(LogCoord, Log, TEXT("현재 위치 : %s"), *Coords[Idx].ToString());
	UE_LOG(LogCoord, Log, TEXT("이동 거리 : %f"), distance);
	FullDistance += distance;
}

void AMyActor::InitTargetCoords()
{
	for (int i = 1; i < 11; i++)
	{
		FVector next = Coords[i - 1];
		if (Task5::Utils::IsSuccess50Percent())
		{
			next.X += Step();
		}
		else
		{
			next.X -= Step();
		}

		if (Task5::Utils::IsSuccess50Percent())
		{
			next.Y += Step();
		}
		else
		{
			next.Y -= Step();
		}
		Coords.Add(next);
	}
}

void AMyActor::EventCheck()
{
	if (Task5::Utils::IsSuccess50Percent())
	{
		EventCounts++;
		UE_LOG(LogCoord, Log, TEXT("이벤트가 발생했습니다!"));
	}
}


void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsArrived == false)
		Move();
}

