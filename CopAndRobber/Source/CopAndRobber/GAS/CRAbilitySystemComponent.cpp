// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CRAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsStatic.h"
#include "Attribute/CRAttributeSet.h"

UCRAbilitySystemComponent::UCRAbilitySystemComponent()
{

}

void UCRAbilitySystemComponent::ApplyInitialEffects()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}
	for (const TSubclassOf<UGameplayEffect>& EffectClass : InitialGameplayEffects)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}

}


void UCRAbilitySystemComponent::GiveInitialAbilities()
{
    
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}
    
	for (const TPair<ECRAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : Abilities)
	{
		if (AbilityPair.Value)
		{
			GiveAbility(FGameplayAbilitySpec(AbilityPair.Value,0,(int32)AbilityPair.Key,nullptr));
		}
	}

	for (const TPair<ECRAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : BasicAbilities)
	{
		if (AbilityPair.Value)
		{
			GiveAbility(FGameplayAbilitySpec(AbilityPair.Value,1,(int32)AbilityPair.Key,nullptr));
		}
	}
}

void UCRAbilitySystemComponent::ApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int level)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return; 
	}
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(GameplayEffect, level, MakeEffectContext());
	ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}


