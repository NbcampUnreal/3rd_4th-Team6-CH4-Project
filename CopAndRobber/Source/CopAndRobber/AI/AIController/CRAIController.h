#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CRAIController.generated.h"

UCLASS()
class COPANDROBBER_API ACRAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category="AI")
	UBehaviorTree* BehaviorTreeAsset;
};
