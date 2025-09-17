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

public:
    //이동 완료 확인 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float AcceptanceRadius = 5.f;

    //최대 이동 반경
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float MaxMoveRadius = 1000.f;

    //최소 이동 거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float MinMoveDistance = 500.f;
    
};
