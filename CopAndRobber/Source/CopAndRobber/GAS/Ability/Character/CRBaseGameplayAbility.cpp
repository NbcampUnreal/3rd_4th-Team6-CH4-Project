// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/CRBaseGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/GameplayTagsStatic.h"
#include "Kismet/GameplayStatics.h"

void UCRBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}






