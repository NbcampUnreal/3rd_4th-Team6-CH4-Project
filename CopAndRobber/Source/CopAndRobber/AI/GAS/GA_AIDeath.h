#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/CRGameplayAbility.h"
#include "GA_AIDeath.generated.h"

UCLASS()
class COPANDROBBER_API UGA_AIDeath : public UCRGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AIDeath();

	virtual void ActivateAbility(
	   const FGameplayAbilitySpecHandle Handle, 
	   const FGameplayAbilityActorInfo* ActorInfo, 
	   const FGameplayAbilityActivationInfo ActivationInfo, 
	   const FGameplayEventData* TriggerEventData
	) override;

	virtual void EndAbility(
	   const FGameplayAbilitySpecHandle Handle,
	   const FGameplayAbilityActorInfo* ActorInfo,
	   const FGameplayAbilityActivationInfo ActivationInfo,
	   bool bReplicateEndAbility,
	   bool bWasCancelled
	   ) override;

protected:
	UFUNCTION()
	void OnDeathSequenceFinished();
	UFUNCTION()
	void OnDeathCancelled();

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage>DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Timing")
	float DeathTime = 3.0f; 
};