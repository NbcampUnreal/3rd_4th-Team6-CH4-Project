#include "CRBTTask_ApproachPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIController/CRAIController.h"
#include "TimerManager.h"

UCRBTTask_ApproachPlayer::UCRBTTask_ApproachPlayer()
{
    bNotifyTick = true; 
}

EBTNodeResult::Type UCRBTTask_ApproachPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp.GetAIOwner());
    if (!AICon || !AICon->HasAuthority())
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
    if (!BBComp)
    {
        return EBTNodeResult::Failed;
    }
    FVector PlayerLocation = BBComp->GetValueAsVector(ACRAIController::BBKey_PlayerLocation);
    if (PlayerLocation.IsNearlyZero())
    {
        return EBTNodeResult::Failed;
    }
    BBComp->SetValueAsVector(ACRAIController::BBKey_TargetLocation, PlayerLocation);
    AICon->MoveToLocation(PlayerLocation, -1.f, false, true, false, false, nullptr, true);

   
    if (OwnerComp.GetWorld())
    {
        OwnerComp.GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &OwnerComp]()
        {
            OnApproachTimerFinished(&OwnerComp);
        }, ApproachDuration, false);
    }

    return EBTNodeResult::InProgress;
}

void UCRBTTask_ApproachPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp.GetAIOwner());
    if (!AICon)
    {
        return;
    }
    UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
    if (!BBComp)
    {
        return;
    }
    FVector PlayerLocation = BBComp->GetValueAsVector(ACRAIController::BBKey_PlayerLocation);
    if (PlayerLocation.IsNearlyZero())
    {
        return;
    }
    FVector CurrentTarget = BBComp->GetValueAsVector(ACRAIController::BBKey_TargetLocation);
    if (!PlayerLocation.Equals(CurrentTarget, 10.f))
    {
        BBComp->SetValueAsVector(ACRAIController::BBKey_TargetLocation, PlayerLocation);
        AICon->MoveToLocation(PlayerLocation, -1.f, false, true, false, false, nullptr, true);
    }
}

void UCRBTTask_ApproachPlayer::OnApproachTimerFinished(UBehaviorTreeComponent* OwnerComp)
{
    if (!OwnerComp)
    {
        return;
    }
    ACRAIController* AICon = Cast<ACRAIController>(OwnerComp->GetAIOwner());
    if (!AICon)
    {
        return;
    }
    AICon->ResetActionIndex(); 
    FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
