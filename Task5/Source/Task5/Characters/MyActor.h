#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class TASK5_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyActor();

protected:
	virtual void BeginPlay() override;

	int32_t Step();

	void Move();

public:	
	virtual void Tick(float DeltaTime) override;

public:
	// Vector와 비슷한 동적 배열 컨테이너
	TArray<FVector2D> Coords;
	int32_t Idx;
};
