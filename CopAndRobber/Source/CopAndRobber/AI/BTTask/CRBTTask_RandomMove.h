#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CRBTTask_RandomMove.generated.h"

UCLASS()
class COPANDROBBER_API UCRBTTask_RandomMove : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UCRBTTask_RandomMove();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    bool bHasStartedMove;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float AcceptanceRadius = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float MaxMoveRadius = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float MinMoveDistance = 500.f;
};
