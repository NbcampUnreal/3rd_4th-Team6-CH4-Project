#include "CRAISpawner.h"
#include "AI/AIController/CRAIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

ACRAISpawner::ACRAISpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACRAISpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnAllAI();
}

void ACRAISpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (ACharacter* AI : SpawnedAIs)
	{
		if (IsValid(AI))
		{
			AI->Destroy();
		}
	}
	SpawnedAIs.Empty();

	Super::EndPlay(EndPlayReason);
}


void ACRAISpawner::SpawnAllAI()
{
	if (!AIClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	int32 Spawned = 0;
	int32 Attempts = 0;

	while (Spawned < SpawnCount && Attempts < SpawnCount * 5)
	{
		FVector SpawnLoc;
		if (GetRandomSpawnLocation(SpawnLoc))
		{
			SpawnLoc = AdjustCapsuleHeight(SpawnLoc);
			FRotator SpawnRot(0.f, FMath::RandRange(0.f, 360.f), 0.f);

			ACharacter* SpawnedChar = SpawnAI(SpawnLoc, SpawnRot);
			if (SpawnedChar)
			{
				SpawnedAIs.Add(SpawnedChar);
				Spawned++;
			}
		}
		Attempts++;
	}
}

bool ACRAISpawner::GetRandomSpawnLocation(FVector& OutLocation) const
{
	UWorld* World = GetWorld();
	if (!World) return false;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys) return false;

	FNavLocation RandomLocation;
	if (NavSys->GetRandomPoint(RandomLocation))
	{
		OutLocation = RandomLocation.Location;
		return true;
	}

	return false;
}

FVector ACRAISpawner::AdjustCapsuleHeight(const FVector& Location) const
{
	if (!AIClass) return Location;

	ACharacter* DefaultChar = AIClass->GetDefaultObject<ACharacter>();
	if (!DefaultChar) return Location;

	float CapsuleHalfHeight = DefaultChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	return Location + FVector(0.f, 0.f, CapsuleHalfHeight);
}

ACharacter* ACRAISpawner::SpawnAI(const FVector& Location, const FRotator& Rotation)
{
	UWorld* World = GetWorld();
	if (!World || !AIClass) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ACharacter* SpawnedChar = World->SpawnActor<ACharacter>(AIClass, Location, Rotation, SpawnParams);
    
	if (SpawnedChar)
	{
		AAIController* NewController = World->SpawnActor<AAIController>(AIControllerClass, Location, Rotation);
       
		if (NewController)
		{
			NewController->Possess(SpawnedChar);
		}
	}
	return SpawnedChar;
}
