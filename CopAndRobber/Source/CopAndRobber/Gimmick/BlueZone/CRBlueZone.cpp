#include "CRBlueZone.h"

#include "Components/CapsuleComponent.h"
#include "CRZoneCountdownComponent.h"
#include "DrawDebugHelpers.h"

ACRBlueZone::ACRBlueZone()
{
	SafeZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SafeZone"));
	SetRootComponent(SafeZone);

	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
}

static UCRZoneCountdownComponent* GetCountdownComp(AActor* Actor)
{
	if (APawn* Pawn = Cast<APawn>(Actor))
	{
		return Pawn->FindComponentByClass<UCRZoneCountdownComponent>();
	}
	return nullptr;                            
}

void ACRBlueZone::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugCapsule(
	GetWorld(),
	SafeZone->GetComponentLocation(),
	SafeZone->GetScaledCapsuleHalfHeight(),
	SafeZone->GetScaledCapsuleRadius(),
	SafeZone->GetComponentQuat(),
	FColor::Cyan, true, -1.f, 0, 2.f
	);
	
	SafeZone->InitCapsuleSize(ZoneRadius, ZoneHalfHeight);

	SafeZone->OnComponentBeginOverlap.AddDynamic(this, &ACRBlueZone::OnZoneBeginOverlap);
	SafeZone->OnComponentEndOverlap.AddDynamic(this, &ACRBlueZone::OnZoneEndOverlap);

	TArray<AActor*> OverlappingActors;
	SafeZone->GetOverlappingActors(OverlappingActors, APawn::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		if (UCRZoneCountdownComponent* Comp = GetCountdownComp(Actor))
		{
			Comp->ServerStopCountdown();
		}
	}
}

void ACRBlueZone::OnZoneBeginOverlap(
	UPrimitiveComponent* Overlapped,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	int32 BodyIndex,
	bool bFromSweep,
	const FHitResult& Sweep
	)
{
	if (!HasAuthority()) return;

	if (UCRZoneCountdownComponent* Zone = GetCountdownComp(Other))
	{
		Zone->ServerStopCountdown();
	}
}

void ACRBlueZone::OnZoneEndOverlap(
	UPrimitiveComponent* Overlapped,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	int32 BodyIndex
	)
{
	if (!HasAuthority()) return;

	if (UCRZoneCountdownComponent* Zone = GetCountdownComp(Other))
	{
		Zone->ServerStartCountdown();
	}
}

