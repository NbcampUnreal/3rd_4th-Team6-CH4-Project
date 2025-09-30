#include "CRBTTask_RandomMove.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIController/CRAIController.h"
#include "Navigation/PathFollowingComponent.h"

UCRBTTask_RandomMove::UCRBTTask_RandomMove()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UCRBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp.GetAIOwner());
    if (!AICon || !AICon->HasAuthority())
    {
        return EBTNodeResult::Failed;
    }

    APawn* AIPawn = AICon->GetPawn();
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AICon->GetWorld());
    if (!NavSys)
    {
        return EBTNodeResult::Failed;
    }
    CurrentWaitTime = 0.0f;
    StuckTimer = 0.0f;
    LastLocation = AIPawn->GetActorLocation();
    FNavLocation RandomLocation;
    if (NavSys->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), MaxMoveRadius, RandomLocation))
    {
        float Distance = FVector::Dist(AIPawn->GetActorLocation(), RandomLocation.Location);
        if (Distance >= MinMoveDistance)
        {
            EPathFollowingRequestResult::Type MoveResult = AICon->MoveToLocation(
                RandomLocation.Location,
                AcceptanceRadius
            );

            if (MoveResult == EPathFollowingRequestResult::Failed)
            {
                return EBTNodeResult::Failed;
            }

            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}
void UCRBTTask_RandomMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp.GetAIOwner());
    if (!AICon)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }
    UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
    if (BBComp && BBComp->GetValueAsBool(ACRAIController::BBKey_bIsPlayerDetected))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    CurrentWaitTime += DeltaSeconds;
    if (CurrentWaitTime >= MaxWaitTime)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UPathFollowingComponent* PFC = AICon->GetPathFollowingComponent();
    if (!PFC)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    EPathFollowingStatus::Type Status = PFC->GetStatus();
    
    if (Status == EPathFollowingStatus::Idle)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }
    
    if (Status == EPathFollowingStatus::Paused)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    APawn* AIPawn = AICon->GetPawn();
    if (AIPawn)
    {
        FVector CurrentLocation = AIPawn->GetActorLocation();
        float MovedDistance = FVector::Dist(CurrentLocation, LastLocation);
        
        if (MovedDistance < 10.0f)
        {
            StuckTimer += DeltaSeconds;
            
            if (StuckTimer >= StuckCheckTime)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }
        }
        else
        {
            StuckTimer = 0.0f;
        }
        
        LastLocation = CurrentLocation;
    }
}