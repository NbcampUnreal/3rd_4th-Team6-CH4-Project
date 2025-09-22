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
	FApproachPlayerTaskMemory* TaskMemory = (FApproachPlayerTaskMemory*)NodeMemory;
	TaskMemory->bFinished = false;

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
	AICon->MoveToLocation(PlayerLocation, 150.f, false, true, false, false, nullptr, true);

	if (UWorld* World = OwnerComp.GetWorld())
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UCRBTTask_ApproachPlayer::OnApproachTimerFinished, &OwnerComp, TaskMemory);
		World->GetTimerManager().SetTimer(TaskMemory->TimerHandle, TimerDel, ApproachDuration, false);
	}

	return EBTNodeResult::InProgress;
}

void UCRBTTask_ApproachPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FApproachPlayerTaskMemory* TaskMemory = (FApproachPlayerTaskMemory*)NodeMemory;
	if (TaskMemory->bFinished)
	{
		return;
	}

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
		AICon->MoveToLocation(PlayerLocation, 150.f, false, true, false, false, nullptr, true);
	}
}

void UCRBTTask_ApproachPlayer::OnApproachTimerFinished(UBehaviorTreeComponent* OwnerComp, FApproachPlayerTaskMemory* TaskMemory)
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

	AICon->StopMovement();
	AICon->ResetActionIndex();
	TaskMemory->bFinished = true;
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
