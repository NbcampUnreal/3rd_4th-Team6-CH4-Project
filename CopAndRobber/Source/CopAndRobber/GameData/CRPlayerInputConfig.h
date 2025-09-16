// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CRPlayerInputConfig.generated.h"

enum class ECRAbilityInputID : uint8;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRPlayerInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = InputAction)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = InputAction)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = InputAction)
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditDefaultsOnly, Category = InputAction)
	TMap<ECRAbilityInputID, UInputAction*> GameplayAbilityInputActions;

	
	UPROPERTY(EditDefaultsOnly, Category = InputMappingContext)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	
};
