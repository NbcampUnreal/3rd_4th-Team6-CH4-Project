#include "CRBTTask_RandomMove.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIController/CRAIController.h"
#include "Navigation/PathFollowingComponent.h"

UCRBTTask_RandomMove::UCRBTTask_RandomMove()
{
    bNotifyTick = true; 
    bHasStartedMove = false; // 이동 시작 여부 플래그
}

EBTNodeResult::Type UCRBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp.GetAIOwner());
    if (!AICon || !AICon->HasAuthority())
        return EBTNodeResult::Failed;

    bHasStartedMove = false; // Task 시작 시 초기화

    APawn* AIPawn = AICon->GetPawn();
    if (!AIPawn) return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AICon->GetWorld());
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation RandomLocation;
    if (NavSys->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), MaxMoveRadius, RandomLocation))
    {
        float Distance = FVector::Dist(AIPawn->GetActorLocation(), RandomLocation.Location);
        if (Distance >= MinMoveDistance)
        {
            AICon->MoveToLocation(RandomLocation.Location, AcceptanceRadius);
            bHasStartedMove = true; // 이동 시작
            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}
void UCRBTTask_RandomMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp.GetAIOwner());
    if (!AICon) return;

    UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
    if (!BBComp) return;

    // 플레이어 감지 시 Task 종료하지 않고 BT가 즉시 시퀀스 전환 가능하게 Tick만 체크
    if (BBComp->GetValueAsBool(ACRAIController::BBKey_bIsPlayerDetected))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    EPathFollowingStatus::Type Status = AICon->GetPathFollowingComponent()->GetStatus();
    if (Status == EPathFollowingStatus::Idle)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
