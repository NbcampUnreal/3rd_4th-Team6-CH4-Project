// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Abilities/GameplayAbility.h"
#include "CRGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


public:
	UCRGameplayAbility();


protected:

	class ACRCharacter* GetOwner() const;
	
	UAnimInstance* GetOwnerAnimInstance() const;
	
};
