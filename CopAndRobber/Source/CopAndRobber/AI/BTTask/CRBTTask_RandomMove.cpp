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
    int32 MaxAttempts = 5;
    bool bFoundValidLocation = false;
    
    for (int32 i = 0; i < MaxAttempts; i++)
    {
        if (NavSys->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), MaxMoveRadius, RandomLocation))
        {
            float Distance = FVector::Dist(AIPawn->GetActorLocation(), RandomLocation.Location);
            
            if (Distance >= MinMoveDistance)
            {
                FNavLocation ProjectedLocation;
                if (NavSys->ProjectPointToNavigation(RandomLocation.Location, ProjectedLocation, FVector(100.0f, 100.0f, 100.0f)))
                {
                    bFoundValidLocation = true;
                    RandomLocation = ProjectedLocation;
                    break;
                }
            }
        }
    }
    
    if (!bFoundValidLocation)
    {
        return EBTNodeResult::Failed;
    }

    EPathFollowingRequestResult::Type MoveResult = AICon->MoveToLocation(
        RandomLocation.Location,
        AcceptanceRadius,
        true,   
        true,  
        true,   
        true,  
        nullptr,
        true    
    );

    if (MoveResult == EPathFollowingRequestResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::InProgress;
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
        AICon->StopMovement();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    APawn* AIPawn = AICon->GetPawn();
    if (!AIPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector CurrentLocation = AIPawn->GetActorLocation();
    float MovedDistance = FVector::Dist(CurrentLocation, LastLocation);
    
    if (MovedDistance < StuckDistanceThreshold)
    {
        StuckTimer += DeltaSeconds;
        
        if (StuckTimer >= StuckCheckTime)
        {
            AICon->StopMovement();
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return;
        }
    }
    else
    {
        StuckTimer = 0.0f;
    }
    
    CurrentWaitTime += DeltaSeconds;
    if (CurrentWaitTime >= MaxWaitTime)
    {
        AICon->StopMovement();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }
    
    LastLocation = CurrentLocation;
}