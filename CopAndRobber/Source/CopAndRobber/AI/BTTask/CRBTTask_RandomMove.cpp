#include "CRBTTask_RandomMove.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIController/CRAIController.h"

UCRBTTask_RandomMove::UCRBTTask_RandomMove()
{
    bNotifyTick = false; 
}

EBTNodeResult::Type UCRBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp.GetAIOwner());
    if (!AICon || !AICon->HasAuthority())
        return EBTNodeResult::Failed;

    APawn* AIPawn = AICon->GetPawn();
    if (!AIPawn)
        return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
    if (!BBComp)
        return EBTNodeResult::Failed;

    int32 ActionIndex = BBComp->GetValueAsInt(ACRAIController::BBKey_ActionIndex);

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

            if (ActionIndex == 0)
            {
                BBComp->SetValueAsBool(ACRAIController::BBKey_bIsActionCooldown, true);
                AICon->StartActionCooldown(7.f);

                AICon->ResetActionIndex();
            }

            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
