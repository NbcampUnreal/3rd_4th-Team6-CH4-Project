// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/GA_Sprint.h"

#include "AbilitySystemComponent.h"

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!K2_CommitAbility())
	{
		return;
	}

	if (K2_HasAuthority())
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(SprintEffect, 1);
		if (SpecHandle.IsValid())
		{
			SprintEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, CurrentActorInfo, CurrentActivationInfo, SpecHandle);
		}
	
	}

	
	
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	
	if (SprintEffectHandle.IsValid() && CurrentActorInfo)
	{
		CurrentActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(SprintEffectHandle);
		SprintEffectHandle = FActiveGameplayEffectHandle(); 
	}
	
}

void UGA_Sprint::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	K2_EndAbility();
	
}
