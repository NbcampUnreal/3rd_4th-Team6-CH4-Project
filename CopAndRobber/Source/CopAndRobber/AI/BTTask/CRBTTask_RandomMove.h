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

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float AcceptanceRadius = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float MaxMoveRadius = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float MinMoveDistance = 300.0f;

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float MaxWaitTime = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float StuckCheckTime = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float StuckDistanceThreshold = 10.0f;

private:
    float CurrentWaitTime;
    float StuckTimer;
    FVector LastLocation;
};