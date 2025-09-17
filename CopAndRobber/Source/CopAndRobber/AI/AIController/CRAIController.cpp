#include "CRAIController.h"
#include "Character/Player/CRPlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

const FName ACRAIController::BBKey_bIsPlayerDetected(TEXT("bIsPlayerDetected"));
const FName ACRAIController::BBKey_ActionIndex(TEXT("ActionIndex"));
const FName ACRAIController::BBKey_TargetPlayer(TEXT("TargetPlayer"));
const FName ACRAIController::BBKey_bIsActionCooldown(TEXT("bIsActionCooldown"));
const FName ACRAIController::BBKey_TargetLocation(TEXT("TargetLocation"));
const FName ACRAIController::BBKey_PlayerLocation(TEXT("PlayerLocation"));

ACRAIController::ACRAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
    SetPerceptionComponent(*PerceptionComp);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 700.f;
    SightConfig->LoseSightRadius = 700.f;
    SightConfig->PeripheralVisionAngleDegrees = 90.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->SetMaxAge(7.f);

    PerceptionComp->ConfigureSense(*SightConfig);
    PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
    PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ACRAIController::OnTargetPerceptionUpdated);

    bDrawDebug = true;
}

void ACRAIController::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority() && BehaviorTreeAsset)
    {
        RunBehaviorTree(BehaviorTreeAsset);
        BlackboardComp = GetBlackboardComponent();
        BlackboardComp->SetValueAsBool(BBKey_bIsActionCooldown, false);
    }
}

void ACRAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bDrawDebug || !GetPawn() || !BlackboardComp || !SightConfig) return;

    DrawDebugSight();

    UpdatePlayerLocation();
}

void ACRAIController::DrawDebugSight()
{
    if (!bDrawDebug || !GetPawn()) return;

    bool bPlayerDetected = BlackboardComp->GetValueAsBool(BBKey_bIsPlayerDetected);
    FVector EyeLocation;
    FRotator EyeRotation;
    GetPawn()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

    FVector Forward = EyeRotation.Vector();
    float HalfAngle = FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees * 0.5f);

    DrawDebugCone(
        GetWorld(),
        EyeLocation,
        Forward,
        SightConfig->SightRadius,
        HalfAngle,
        HalfAngle,
        12,
        bPlayerDetected ? FColor::Red : FColor::Green,
        false, 0, 0, 0
    );
}

void ACRAIController::UpdatePlayerLocation()
{
    AActor* TargetPlayer = Cast<AActor>(BlackboardComp->GetValueAsObject(BBKey_TargetPlayer));
    if (TargetPlayer)
    {
        BlackboardComp->SetValueAsVector(BBKey_PlayerLocation, TargetPlayer->GetActorLocation());
    }
}

void ACRAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!Actor || !BlackboardComp) return;

    ACRPlayerCharacter* PlayerChar = Cast<ACRPlayerCharacter>(Actor);
    if (!PlayerChar) return;

    bool bSensed = Stimulus.WasSuccessfullySensed();
    BlackboardComp->SetValueAsBool(BBKey_bIsPlayerDetected, bSensed);

    if (bSensed)
    {
        BlackboardComp->SetValueAsObject(BBKey_TargetPlayer, Actor);
    }
    else
    {
        BlackboardComp->ClearValue(BBKey_TargetPlayer);
    }
    UpdateRandomActionIndex();
}

void ACRAIController::UpdateRandomActionIndex()
{
    if (!BlackboardComp) return;

    bool bPlayerDetected = BlackboardComp->GetValueAsBool(BBKey_bIsPlayerDetected);
    bool bIsCooldown = BlackboardComp->GetValueAsBool(BBKey_bIsActionCooldown);

    if (bPlayerDetected && !bIsCooldown)
    {
        int32 NewIndex = FMath::RandRange(0, 2);
        BlackboardComp->SetValueAsInt(BBKey_ActionIndex, NewIndex);
    }
}

void ACRAIController::StartActionCooldown(float CooldownTime)
{
    if (!BlackboardComp) return;

    BlackboardComp->SetValueAsBool(BBKey_bIsActionCooldown, true);

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
    {
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(BBKey_bIsActionCooldown, false);
            UpdateRandomActionIndex();
        }
    }, CooldownTime, false);
}
