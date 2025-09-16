// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GenericTeamAgentInterface.h"
#include "CRCharacter.generated.h"

class UCRAttributeSet;
class UCRAbilitySystemComponent;

UCLASS()
class COPANDROBBER_API ACRCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ACRCharacter();
	void ServerSide();

	
protected:

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;


#pragma  region  GAS
protected:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void OnSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnStunTagChanged(FGameplayTag Tag, int32 NewCount);
	void OnDeathTagChanged(FGameplayTag Tag, int32 NewCount);
	virtual void BindingChangeDelegate();
protected:
	UPROPERTY(VisibleAnywhere,  Category= "GAS")
	TObjectPtr<UCRAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,  Category= "GAS")
	TObjectPtr<UCRAttributeSet> AttributeSet;

#pragma endregion

#pragma region Status
protected:
	bool IsDead() const;
	virtual void OnStun();
	virtual void RecoverStun();

	virtual void OnDeath();
	


protected:
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	TObjectPtr<UAnimMontage> StunMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	TObjectPtr<UAnimMontage> DeathMontage;

	
	
	
#pragma endregion


};
