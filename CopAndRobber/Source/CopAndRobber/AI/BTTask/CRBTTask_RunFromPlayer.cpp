#include "CRBTTask_RunFromPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIController/CRAIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "TimerManager.h"

UCRBTTask_RunFromPlayer::UCRBTTask_RunFromPlayer()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UCRBTTask_RunFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp.GetAIOwner());
    if (!AICon || !AICon->GetBlackboardComponent())
        return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
    if (!BBComp->GetValueAsBool(ACRAIController::BBKey_bIsPlayerDetected))
        return EBTNodeResult::Failed;

    if (BBComp->GetValueAsBool(ACRAIController::BBKey_bIsActionCooldown))
        return EBTNodeResult::Failed;

    APawn* AIPawn = AICon->GetPawn();
    if (!AIPawn) return EBTNodeResult::Failed;

    FVector AIPos = AIPawn->GetActorLocation();
    FVector PlayerPos = BBComp->GetValueAsVector(ACRAIController::BBKey_PlayerLocation);
    FVector AwayDir = (AIPos - PlayerPos).GetSafeNormal();
    if (AwayDir.IsNearlyZero())
        AwayDir = FMath::VRand();

    FVector AwayCenter = AIPos + AwayDir * 600.f;
    FNavLocation NavLoc;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AICon->GetWorld());
    if (NavSys && NavSys->GetRandomReachablePointInRadius(AwayCenter, 300.f, NavLoc))
    {
        BBComp->SetValueAsVector(ACRAIController::BBKey_TargetLocation, NavLoc.Location);
        if (AICon->HasAuthority())
        {
            AICon->MoveToLocation(NavLoc.Location, -1.f, false, true, false, false, nullptr, true);

            BBComp->SetValueAsBool(ACRAIController::BBKey_bIsActionCooldown, true);
            FTimerHandle TimerHandle;
            AICon->GetWorldTimerManager().SetTimer(TimerHandle, [BBComp]()
            {
                BBComp->SetValueAsBool(ACRAIController::BBKey_bIsActionCooldown, false);
            }, 7.f, false);
        }

        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

void UCRBTTask_RunFromPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp.GetAIOwner());
    if (!AICon)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    EPathFollowingStatus::Type Status = AICon->GetPathFollowingComponent()->GetStatus();
    if (Status == EPathFollowingStatus::Idle)
    {
        UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
        if (BBComp)
        {
            BBComp->SetValueAsVector(ACRAIController::BBKey_TargetLocation, FVector::ZeroVector);
        }

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
