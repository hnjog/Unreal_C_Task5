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

public:	
	virtual void Tick(float DeltaTime) override;

public:
	TArray<FVector2D> Coords;

};
