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
}

void ACRAISpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (ACRAICharacter* AI : SpawnedAIs)
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
	if (!HasAuthority() || !AIClass) 
	{
		return;
	}

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
			
			ACRAICharacter* SpawnedChar = SpawnAI(SpawnLoc, SpawnRot);
			if (SpawnedChar)
			{
				Spawned++; 
			}
		}
		Attempts++;
	}
}

bool ACRAISpawner::GetRandomSpawnLocation(FVector& OutLocation) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		return false;
	}
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
	if (!AIClass)
	{
		return Location;
	}
	ACRAICharacter* DefaultChar = AIClass->GetDefaultObject<ACRAICharacter>();
	if (!DefaultChar)
	{
		return Location;
	}
	float CapsuleHalfHeight = DefaultChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	return Location + FVector(0.f, 0.f, CapsuleHalfHeight);
}


ACRAICharacter* ACRAISpawner::SpawnAI(const FVector& Location, const FRotator& Rotation)
{
	UWorld* World = GetWorld();
	if (!World || !AIClass)
	{
		return nullptr;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
    
	if (HasAuthority())
	{
		ACRAICharacter* SpawnedChar = World->SpawnActor<ACRAICharacter>(AIClass, Location, Rotation, SpawnParams);
		if (SpawnedChar)
		{
			SpawnedAIs.Add(SpawnedChar);

			ACRAIController* NewController = World->SpawnActor<ACRAIController>(AIControllerClass, Location, Rotation);
			if (NewController)
			{
				NewController->Possess(SpawnedChar);
			}
		}
		return SpawnedChar;
	}
	return nullptr;
}

