// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CRCharacter.generated.h"

class UCRAttributeSet;
class UCRAbilitySystemComponent;

UCLASS()
class COPANDROBBER_API ACRCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ACRCharacter();

	
protected:

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	



#pragma  region  GAS
protected:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void ServerSide();
	void ClientSide();
protected:
	UPROPERTY(VisibleAnywhere,  Category= "GAS")
	TObjectPtr<UCRAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,  Category= "GAS")
	TObjectPtr<UCRAttributeSet> AttributeSet;

#pragma endregion

	

};
