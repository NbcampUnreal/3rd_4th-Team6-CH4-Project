//CRItemBase.cpp

#include "CRItemBase.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

ACRItemBase::ACRItemBase()
{
	bReplicates = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollision"));
	SetRootComponent(Collision);
	Collision->InitSphereRadius(50.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetGenerateOverlapEvents(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	PrimaryActorTick.bCanEverTick = false;
}

void ACRItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (!Collision) return;
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACRItemBase::OnOverlapBegin);
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
	
	if (APawn* Pawn = Cast<APawn>(OtherActor))
	{
		bPicked = true;
		if (Collision)
		{
			Collision->SetGenerateOverlapEvents(false);
			Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		OnRep_Picked();

		Activate(OtherActor);
		Destroy();
	}
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

void ACRItemBase::Activate(AActor* Player){}

