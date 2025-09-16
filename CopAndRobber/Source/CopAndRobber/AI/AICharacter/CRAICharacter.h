// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CRCharacter.h"
#include "CRAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class COPANDROBBER_API ACRAICharacter : public ACRCharacter
{
	GENERATED_BODY()
public:
	ACRAICharacter();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAnimMontage* DeathMontage;
};
