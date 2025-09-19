// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagsStatic.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class COPANDROBBER_API UGameplayTagsStatic : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()
	
public:
	static FGameplayTag GetBasicAttackAbilityTag();

	static FGameplayTag GetDeadStatTag();
	static FGameplayTag GetStunStatTag();

	static FGameplayTag GetGenericDamagePointTag();

	static FGameplayTag GetHitStatTag();
};
