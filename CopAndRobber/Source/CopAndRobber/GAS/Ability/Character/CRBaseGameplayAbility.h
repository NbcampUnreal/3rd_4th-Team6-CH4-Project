// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/CRGameplayAbility.h"
#include "CRBaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRBaseGameplayAbility : public UCRGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


};
