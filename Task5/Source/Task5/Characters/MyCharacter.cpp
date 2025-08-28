#include "MyCharacter.h"
#include "../LogTests.h"
#include "../MyUtils.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"

AMyCharacter::AMyCharacter()
	:Idx(0),
	IsFinish(false),
	EventCounts(0),
	FullDistance(0.0)
{
	PrimaryActorTick.bCanEverTick = true;
	NextCoord = FVector();

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsFinish == false)
	{
		if (CheckArrive())
		{
			Move();
		}
		/*else
		{
			GoToNext();
		}*/
	}
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetNextCoord();
	Idx = 1;

	GoToNext();
}

int32_t AMyCharacter::Step()
{
	return FMath::RandRange(0, 1);
}

PRAGMA_DISABLE_OPTIMIZATION

void AMyCharacter::Move()
{
	if (Idx >= 11)
	{
		IsFinish = true;
		UE_LOG(LogCoord, Log, TEXT("총 발생 이벤트 횟수 : % d"), EventCounts);
		UE_LOG(LogCoord, Log, TEXT("총 이동 거리 : %f"), FullDistance);
		return;
	}

	EventCheck();

	Idx++;

	SetNextCoord();

	UE_LOG(LogCoord, Log, TEXT("현재 위치 : %s"), *GetActorLocation().ToString());
	
	GoToNext();
}

void AMyCharacter::GoToNext()
{
	if (AController* controller = GetController())
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, NextCoord);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(controller, FXCursor, NextCoord, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

PRAGMA_ENABLE_OPTIMIZATION

void AMyCharacter::SetNextCoord()
{
	const int32_t value = 200;

	double distance = Task5::Utils::GetDistance(GetActorLocation(), NextCoord);
	UE_LOG(LogCoord, Log, TEXT("이동 거리 : %f"), distance);
	FullDistance += distance;

	if (Task5::Utils::IsSuccess50Percent())
	{
		NextCoord.X += Step() * value;
	}
	else
	{
		NextCoord.X -= Step() * value;
	}

	if (Task5::Utils::IsSuccess50Percent())
	{
		NextCoord.Y += Step() * value;
	}
	else
	{
		NextCoord.Y -= Step() * value;
	}
}

void AMyCharacter::EventCheck()
{
	if (Task5::Utils::IsSuccess50Percent())
	{
		EventCounts++;
		FVector nowLoc = GetActorLocation();
		nowLoc.Z += 100.0f;
		if (AController* controller = GetController())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(controller, FXEvent, nowLoc, FRotator::ZeroRotator, FVector(2.f, 2.f, 2.f), true, true, ENCPoolMethod::None, true);
		}
		UE_LOG(LogCoord, Log, TEXT("이벤트가 발생했습니다!"));
	}
}

PRAGMA_DISABLE_OPTIMIZATION

bool AMyCharacter::CheckArrive()
{
	FVector nowLoc = GetActorLocation();
	if (Task5::Utils::GetDistance(nowLoc, NextCoord) <= 10.0 + GetCapsuleComponent()->GetScaledCapsuleRadius())
		return true;

	return false;
}

PRAGMA_ENABLE_OPTIMIZATION