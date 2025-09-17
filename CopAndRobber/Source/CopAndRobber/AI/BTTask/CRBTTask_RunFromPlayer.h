#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CRBTTask_RunFromPlayer.generated.h"

UCLASS()
class COPANDROBBER_API UCRBTTask_RunFromPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCRBTTask_RunFromPlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
