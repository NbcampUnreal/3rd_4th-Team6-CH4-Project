//CRBlueZone.cpp

#include "CRBlueZone.h"

#include "Components/CapsuleComponent.h"
#include "CRZoneCountdownComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

static constexpr float kBaseCylinderRadius = 50.f;
static constexpr float kBaseCylinderHeight = 100.f;

ACRBlueZone::ACRBlueZone()
{
	SafeZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SafeZone"));
	SetRootComponent(SafeZone);

	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
	ZoneMesh->SetupAttachment(SafeZone);
	ZoneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ACRBlueZone::SetZoneRadius(float NewRadius)
{
	ZoneRadius = NewRadius;
	
	SafeZone->SetCapsuleRadius(ZoneRadius);
	SyncMeshToCapsuleRadius();
	if (ZoneMID)
	{
		ZoneMID->SetScalarParameterValue(TEXT("ZoneRadius"), ZoneRadius);
	}
}

void ACRBlueZone::OnRep_ZoneRadius()
{
	SafeZone->InitCapsuleSize(ZoneRadius, ZoneHalfHeight);

	const float XY = ZoneRadius / kBaseCylinderRadius;
	const float Z  = (ZoneHalfHeight * 2.f) / kBaseCylinderHeight;
	ZoneMesh->SetRelativeScale3D(FVector(XY, XY, Z));

	ApplyZoneMaterialOnce();
	if (ZoneMID)
	{
		ZoneMID->SetScalarParameterValue(TEXT("ZoneRadius"), ZoneRadius);
	}
}

void ACRBlueZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACRBlueZone, ZoneRadius);
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

	ApplyZoneMaterialOnce();
	
	SafeZone->InitCapsuleSize(ZoneRadius, ZoneHalfHeight);
	SyncMeshToCapsuleRadius();

	if (HasAuthority())
	{
		SafeZone->OnComponentBeginOverlap.AddDynamic(this, &ACRBlueZone::OnZoneBeginOverlap);
		SafeZone->OnComponentEndOverlap.AddDynamic(this, &ACRBlueZone::OnZoneEndOverlap);

		StartRadius = SafeZone->GetUnscaledCapsuleRadius();

		GetWorld()->GetTimerManager().SetTimer(
			ShrinkTimer,
			this,
			&ACRBlueZone::StartShrink,
			ShrinkDelay,
			false
		);
	}

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

void ACRBlueZone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasAuthority() || !bIsShrinking) return;
	ElapsedTime += DeltaSeconds;
	float Alpha = FMath::Clamp(ElapsedTime / ShrinkDuration, 0.f, 1.f);
	float NewRadius = FMath::Lerp(StartRadius, TargetRadius, Alpha);
	SetZoneRadius(NewRadius);
	
	if (Alpha >= 1.f)
	{
		bIsShrinking = false;
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

void ACRBlueZone::SyncMeshToCapsuleRadius()
{
	if (!SafeZone || !ZoneMesh) return;
	
	const float CapsuleRadius = SafeZone->GetScaledCapsuleRadius();
	
	const float XYScale = CapsuleRadius / kBaseCylinderRadius;
	
	const FVector Cur = ZoneMesh->GetRelativeScale3D();
	ZoneMesh->SetRelativeScale3D(FVector(XYScale, XYScale, ZoneHalfHeight));
}

void ACRBlueZone::ApplyZoneMaterialOnce()
{
	if (!ZoneMesh || ZoneMID) return;

	UMaterialInterface* Base = ZoneBaseMaterial ? ZoneBaseMaterial : ZoneMesh->GetMaterial(0);
	if (!Base || Base->GetBlendMode() != BLEND_Translucent) return;

	ZoneMID = UMaterialInstanceDynamic::Create(Base, this);
	ZoneMesh->SetMaterial(0, ZoneMID);
	ZoneMID->SetScalarParameterValue(TEXT("ZoneRadius"), ZoneRadius);
}

void ACRBlueZone::StartShrink()
{
	bIsShrinking = true;
	ElapsedTime = 0.f;
}
