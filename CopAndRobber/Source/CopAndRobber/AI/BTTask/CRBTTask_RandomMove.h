// CRBTTask_RandomMove.h
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CRBTTask_RandomMove.generated.h"
/*
 * 플레이어가 보이지 않으면 NavMesh 위 랜덤 좌표로 이동
 */
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
