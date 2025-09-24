// CRItemSpawnVolume.cpp

#include "CRItemSpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Gimmick/Item/Base/CRItemBase.h"
#include "Kismet/KismetMathLibrary.h"

static const float TraceUp   = 3000.f;
static const float TraceDown = 6000.f;

ACRItemSpawnVolume::ACRItemSpawnVolume()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetBoxExtent(FVector(100.0f, 100.0f, 10.0f));
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PrimaryActorTick.bCanEverTick = false;
}

void ACRItemSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority()) return;

	GetWorldTimerManager().SetTimer(
		SpawnLoopTimer,
		this,
		&ACRItemSpawnVolume::SpawnLoop,
		SpawnInterval,
		true,
		SpawnInterval
	);
}

void ACRItemSpawnVolume::SpawnLoop()
{
	CleanSpawnedItemPtr();

	if (CanSpawnItem())
	{
		SpawnItem();
	}
}

void ACRItemSpawnVolume::DoPendingRespawn()
{
	CleanSpawnedItemPtr();

	if (PendingRespawns > 0 && CanSpawnItem())
	{
		SpawnItem();
		PendingRespawns--;
	}

	if (PendingRespawns > 0)
	{
		GetWorldTimerManager().SetTimer(
			RespawnTimer,
			this,
			&ACRItemSpawnVolume::DoPendingRespawn,
			SpawnInterval,
			false
			);
	}
}

bool ACRItemSpawnVolume::CanSpawnItem() const
{
	int32 SpawnedItemCount = 0;
	for (auto& It : SpawnedItems) if (It.IsValid()) ++SpawnedItemCount;
	return SpawnedItemCount < MaxSpawnCount;
}

void ACRItemSpawnVolume::CleanSpawnedItemPtr()
{
	for (int32 i = SpawnedItems.Num()-1; i >= 0; --i)
	{
		if (!SpawnedItems[i].IsValid())
		{
			SpawnedItems.RemoveAt(i);
		}
	}
}

ACRItemBase* ACRItemSpawnVolume::SpawnItem()
{
	TSubclassOf<ACRItemBase> ItemClass = PickItemClass();
	if (!*ItemClass) return nullptr;

	const FVector Center = Box->GetComponentLocation();
	const FVector Extent = Box->GetScaledBoxExtent();
	const FVector RandLocal = UKismetMathLibrary::RandomPointInBoundingBox(FVector::ZeroVector, Extent);
	const FVector XY = Center + FVector(RandLocal.X, RandLocal.Y, 0.f);
	
	FHitResult Hit;
	if (!FindGroundedLocation(XY, Hit)) return nullptr;

	const float HalfHeight = GetItemHalfHeight(ItemClass);
	const float SmallLift = 2.f;
	const FVector PlaceLoc = Hit.ImpactPoint + Hit.ImpactNormal * (HalfHeight + SmallLift);

	const FRotator Rot = FRotator::ZeroRotator;
	const FTransform SpawnXf(Rot, PlaceLoc);
	
	FActorSpawnParameters P;
	P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACRItemBase* Item = GetWorld()->SpawnActor<ACRItemBase>(ItemClass, SpawnXf, P);
	
	if (Item)
	{
		SpawnedItems.Add(Item);
		Item->OnDestroyed.AddDynamic(this, &ACRItemSpawnVolume::OnItemDestroyed);
	}
	return Item;
}

TSubclassOf<ACRItemBase> ACRItemSpawnVolume::PickItemClass() const
{
	if (ItemClasses.Num() == 0) return nullptr;

	const int32 Idx = FMath::RandRange(0, ItemClasses.Num() - 1);
	return ItemClasses[Idx];
}

void ACRItemSpawnVolume::OnItemDestroyed(AActor* Dead)
{
	if (!HasAuthority()) return;

	for (int32 i = SpawnedItems.Num()-1; i >= 0; --i)
	{
		if (!SpawnedItems[i].IsValid() || SpawnedItems[i].Get() == Dead)
		{
			SpawnedItems.RemoveAt(i);
		}
	}

	PendingRespawns++;

	if (!GetWorldTimerManager().IsTimerActive(RespawnTimer))
	{
		GetWorldTimerManager().SetTimer(
			RespawnTimer,
			this,
			&ACRItemSpawnVolume::DoPendingRespawn,
			SpawnInterval,
			false
		);
	}
}

bool ACRItemSpawnVolume::FindGroundedLocation(const FVector& XY, FHitResult& OutHit) const
{
	const FVector Start = XY + FVector(0, 0, TraceUp);
	const FVector End = XY - FVector(0, 0, TraceDown);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(ItemGroundTrace), false);
	Params.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, Params);
}

float ACRItemSpawnVolume::GetItemHalfHeight(TSubclassOf<ACRItemBase> ItemClass) const
{
	if (!*ItemClass) return 50.f;

	if (const ACRItemBase* CDO = Cast<ACRItemBase>(ItemClass->GetDefaultObject()))
	{
		const FBox Bounds =CDO->GetComponentsBoundingBox(true);
		if (Bounds.IsValid) return Bounds.GetExtent().Z;
	}
	return 50.f;
}