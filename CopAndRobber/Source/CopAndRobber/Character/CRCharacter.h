// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "CRCharacter.generated.h"

class UGameplayEffect;
class UCRAttributeSet;
class UCRAbilitySystemComponent;

UCLASS()
class COPANDROBBER_API ACRCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ACRCharacter();
	

	
protected:

	virtual void ServerSideInit();
		
	virtual void BeginPlay() override;

#pragma  region  GAS
protected:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UCRAbilitySystemComponent* GetCRAbilitySystemComponent() const;
	bool IsAbilitySystemInitialized() const;
	void OnSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnStunTagChanged(FGameplayTag Tag, int32 NewCount);
	void OnDeathTagChanged(FGameplayTag Tag, int32 NewCount);
	virtual void BindingChangeDelegate();
protected:
	UPROPERTY(Transient)
	TObjectPtr<UCRAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient)
	TObjectPtr<UCRAttributeSet> AttributeSet;

#pragma endregion

#pragma region Status
protected:
	virtual bool IsDead() const;
	virtual void OnStun();
	virtual void RecoverStun();
	virtual void OnDeath();
	void UpdatedHealth(const FOnAttributeChangeData& OnAttributeChangeData);

	UPROPERTY(EditDefaultsOnly, Category = "Dead Effects")
	TSubclassOf<UGameplayEffect> DeadEffect;
#pragma endregion


};
