// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CRAbilitySystemComponent.h"

void UCRAbilitySystemComponent::ApplyInitialEffects()
{
	
}

void UCRAbilitySystemComponent::ApplyFullStatEffect()
{
	if (IsValid(FullStatEffect))
	{
		ApplyAbilityEffect(FullStatEffect);
	}
}

void UCRAbilitySystemComponent::GiveInitialAbilities()
{
	if (!CheckAuth())
	{
		return;
	}
	for (const TSubclassOf<UGameplayAbility>& ab : Abilities)
	{
		GiveAbility(FGameplayAbilitySpec(ab, 1,1,nullptr));
	}
}



void UCRAbilitySystemComponent::ApplyAbilityEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int level)
{
	if (!CheckAuth())
	{
		return; 
	}
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(GameplayEffect, level, MakeEffectContext());
	ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

bool UCRAbilitySystemComponent::CheckAuth()
{
	return (GetOwner()&& GetOwner()->HasAuthority());
}
