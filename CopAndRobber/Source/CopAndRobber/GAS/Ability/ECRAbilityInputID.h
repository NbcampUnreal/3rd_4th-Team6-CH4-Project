// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECRAbilityInputID.generated.h"

UENUM(BlueprintType)
enum class ECRAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),
	BasicAttack		UMETA(DisplayName = "BasicAttack"),
	AbilityOne		UMETA(DisplayName = "AbilityOne"),
	AbilityTwo		UMETA(DisplayName = "AbilityTwo"),
	AbilityThree	UMETA(DisplayName = "AbilityThree"),
	AbilityFour		UMETA(DisplayName = "AbilityFour")
};
