#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CRBTTask_ApproachPlayer.generated.h"

USTRUCT()
struct FApproachPlayerTaskMemory
{
	GENERATED_BODY()

	bool bFinished = false;
	FTimerHandle TimerHandle;
};

UCLASS()
class COPANDROBBER_API UCRBTTask_ApproachPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCRBTTask_ApproachPlayer();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override
	{
		return sizeof(FApproachPlayerTaskMemory);
	}

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float ApproachDuration = 3.f;

	void OnApproachTimerFinished(UBehaviorTreeComponent* OwnerComp, FApproachPlayerTaskMemory* TaskMemory);
};
