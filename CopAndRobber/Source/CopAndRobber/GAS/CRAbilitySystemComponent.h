// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CRAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void ApplyInitialEffects();
	void ApplyFullStatEffect();
	void GiveInitialAbilities();

private:
	void ApplyAbilityEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int level =1);
	bool CheckAuth();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> FullStatEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	
};
