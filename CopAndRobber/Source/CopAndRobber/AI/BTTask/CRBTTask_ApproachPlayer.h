#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CRBTTask_ApproachPlayer.generated.h"

UCLASS()
class COPANDROBBER_API UCRBTTask_ApproachPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCRBTTask_ApproachPlayer();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float ApproachDuration;

	void OnApproachTimerFinished(UBehaviorTreeComponent* OwnerComp);

private:
	FTimerHandle TimerHandle;
};
