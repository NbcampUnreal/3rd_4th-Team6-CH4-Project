// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CRCharacter.h"
#include "CRAICharacter.generated.h"

class UCRAbilitySystemComponent;
class UCRAttributeSet;
class UBehaviorTree;

UCLASS()
class COPANDROBBER_API ACRAICharacter : public ACRCharacter
{
	GENERATED_BODY()

public:
	ACRAICharacter();

protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TArray<TSubclassOf<class UGameplayAbility>> AIAbilities;
	void InitializeAI();
	virtual void PossessedBy(AController* NewController) override;
private:
	bool bIsInitialized = false;
};
