// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS/Ability/ECRAbilityInputID.h"
#include "CRAbilitySystemComponent.generated.h"


/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	UCRAbilitySystemComponent();
	void ApplyInitialEffects();
	void GiveInitialAbilities();
	void ApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int level = 1);
	void RemoveAllAbilities();

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects" )
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects" )
	TMap<ECRAbilityInputID , TSubclassOf<UGameplayAbility>> Abilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects" )
	TMap<ECRAbilityInputID , TSubclassOf<UGameplayAbility>> BasicAbilities;
	
};
