#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CRAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBlackboardComponent;
class UBehaviorTree;

UCLASS()
class COPANDROBBER_API ACRAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACRAIController();
	
	// 블랙보드 키
	static const FName BBKey_bIsPlayerDetected;
	static const FName BBKey_ActionIndex;
	static const FName BBKey_TargetPlayer;
	static const FName BBKey_TargetLocation;
	static const FName BBKey_PlayerLocation;
	static const FName BBKey_IsHit;

	void ResetActionIndex();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void DrawDebugSight();
	void UpdatePlayerLocation();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UBlackboardComponent> BlackboardComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Debug")
	bool bDrawDebug;


	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	void UpdateRandomActionIndex();

};
