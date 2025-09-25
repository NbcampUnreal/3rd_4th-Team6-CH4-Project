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

void UCRAbilitySystemComponent::RemoveAllAbilities()
{

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		AbilitiesToRemove.Add(Spec.Handle);
	}
	for (const FGameplayAbilitySpecHandle& Handle : AbilitiesToRemove)
	{
		if (Handle.IsValid())
		{
			ClearAbility(Handle); 
		}
	}
}

void UCRAbilitySystemComponent::RemoveAbilities() // Basic 제외 
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
    
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
	
		bool bIsBasicAbility = false;
		for (const TPair<ECRAbilityInputID, TSubclassOf<UGameplayAbility>>& BasicPair : BasicAbilities)
		{
			if (BasicPair.Value && Spec.Ability && Spec.Ability->GetClass() == BasicPair.Value)
			{
				bIsBasicAbility = true;
				break;
			}
		}
		
		if (!bIsBasicAbility)
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}
    
	for (const FGameplayAbilitySpecHandle& Handle : AbilitiesToRemove)
	{
		if (Handle.IsValid())
		{
			ClearAbility(Handle); 
		}
	}
}
