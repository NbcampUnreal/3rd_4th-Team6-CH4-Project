#include "CRBTTask_RandomMove.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"

UCRBTTask_RandomMove::UCRBTTask_RandomMove()
{
    bNotifyTick = false;
}

EBTNodeResult::Type UCRBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon || !AICon->HasAuthority())
        return EBTNodeResult::Failed;

    APawn* AIPawn = AICon->GetPawn();
    if (!AIPawn)
        return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AICon->GetWorld());
    if (!NavSys)
        return EBTNodeResult::Failed;

    FNavLocation RandomLocation;
    if (NavSys->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), MaxMoveRadius, RandomLocation))
    {
        float Distance = FVector::Dist(AIPawn->GetActorLocation(), RandomLocation.Location);
        if (Distance >= MinMoveDistance)
        {
            AICon->MoveToLocation(RandomLocation.Location, AcceptanceRadius);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
