#include "CRDoor.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


ACRDoor::ACRDoor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;
    bReplicates = true;

  
    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneRoot;

   
    FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
    FrameMesh->SetupAttachment(RootComponent);

  PivotComp = CreateDefaultSubobject<USceneComponent>(TEXT("PivotComp"));
    PivotComp->SetupAttachment(RootComponent);
    
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(PivotComp);

    bIsDoorOpen = false;
}

void ACRDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsMoving)
    {
        return;
    }

    CurrentRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, DoorOpenSpeed);
    PivotComp->SetRelativeRotation(CurrentRotation);

    if (CurrentRotation.Equals(TargetRotation, 0.1f))
    {
        bIsMoving = false;
        SetActorTickEnabled(false);
    }
}

void ACRDoor::BeginPlay()
{
    Super::BeginPlay();

    if (!DoorMesh || !DoorMesh->GetStaticMesh())
    {
        return;
    }
    FBox LocalBounds = DoorMesh->GetStaticMesh()->GetBoundingBox();
    
    float HingeX = LocalBounds.Min.X;

    float CenterY = (LocalBounds.Min.Y + LocalBounds.Max.Y) * 0.5f;
    float CenterZ = (LocalBounds.Min.Z + LocalBounds.Max.Z) * 0.5f;

    FVector HingeOffset(HingeX, CenterY, CenterZ);
    PivotComp->SetRelativeLocation(HingeOffset);
    DoorMesh->SetRelativeLocation(-HingeOffset);
}

void ACRDoor::ToggleDoor()
{
    if (bIsMoving) 
    {
        return; 
    }
    SetDoorOpen(!bIsDoorOpen);
}


void ACRDoor::StartDoorMovement()
{
    TargetRotation = bIsDoorOpen ? FRotator(0.f, 90.f, 0.f) : FRotator(0.f, 0.f, 0.f);
    CurrentRotation = PivotComp->GetRelativeRotation();
    bIsMoving = true;
    
    SetActorTickEnabled(true); 
}

void ACRDoor::SetDoorOpen(bool bOpen)
{
    if (bIsDoorOpen == bOpen)
    {
        return;
    }
    bIsDoorOpen = bOpen;
    

    StartDoorMovement();
}


void ACRDoor::OnRep_DoorOpen()
{
 
     if (bIsDoorOpen && DoorOpenSound)
     {
         UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation(), 1.f, 1.f, 0.f, DoorAttenuation);
     }
     else if (!bIsDoorOpen && DoorCloseSound)
    { 
         UGameplayStatics::PlaySoundAtLocation(this, DoorCloseSound, GetActorLocation(), 1.f, 1.f, 0.f, DoorAttenuation);
    }
    StartDoorMovement();
}

void ACRDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACRDoor, bIsDoorOpen);
}
