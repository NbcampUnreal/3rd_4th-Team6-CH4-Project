// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/CRGameplayAbility.h"
#include "GAP_Death.generated.h"

/**
 * 
 */
UCLASS()
class COPANDROBBER_API UGAP_Death : public UCRGameplayAbility
{
	GENERATED_BODY()

public:
	UGAP_Death();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void EndDeathAbility();

protected:
	
	

	UPROPERTY(EditDefaultsOnly, Category = "Time")
	float DeathTime;
	
	FTimerHandle DeathTimerHandle; 
	

};
