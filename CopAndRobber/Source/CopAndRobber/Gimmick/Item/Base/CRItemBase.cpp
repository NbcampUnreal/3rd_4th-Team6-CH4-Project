//CRItemBase.cpp

#include "CRItemBase.h"

#include "Components/BoxComponent.h"

ACRItemBase::ACRItemBase()
{
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	Collision->SetCollisionProfileName(TEXT("OverlapAll"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);

	Duration = 0.f;
	IsActive = false;
	
	
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

void ACRItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                 bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsActive) return;
	
	if (OtherActor->ActorHasTag("Player"))
	{
		Activate();
		if (Duration > 0)
		{
			GetWorldTimerManager().SetTimer(DurationHandle, this, &ACRItemBase::Deactivate, Duration);
		}
		Destroy();
	}
}

void ACRItemBase::Activate()
{
	Super::Activate();
}

void ACRItemBase::Deactivate()
{
	Super::Deactivate();
}