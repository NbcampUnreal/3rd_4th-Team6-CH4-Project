//CRItemBase.cpp

#include "CRItemBase.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

ACRItemBase::ACRItemBase()
{
	bReplicates = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->InitSphereRadius(50.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	
	PrimaryActorTick.bCanEverTick = false;
}

void ACRItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (Collision)
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ACRItemBase::OnOverlapBegin);
	}
}

void ACRItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACRItemBase, bPicked);
}

void ACRItemBase::OnRep_Picked()
{
	ApplyPickedVisuals(bPicked);
}

void ACRItemBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (bPicked || !OtherActor) return;
	
	if (!OtherActor->ActorHasTag("Player")) return;

	bPicked = true;
	OnRep_Picked();

	Activate(OtherActor);
	Destroy();
}

void ACRItemBase::ApplyPickedVisuals(bool bHide)
{
	if (Collision)
	{
		Collision->SetGenerateOverlapEvents(!bHide);
		Collision->SetCollisionEnabled(bHide ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly);
	}
	if (Mesh)
	{
		Mesh->SetHiddenInGame(bHide);
		Mesh->SetVisibility(!bHide, true);
	}
}

void ACRItemBase::Activate(AActor* Player)
{
	Super::Activate(Player);
}

