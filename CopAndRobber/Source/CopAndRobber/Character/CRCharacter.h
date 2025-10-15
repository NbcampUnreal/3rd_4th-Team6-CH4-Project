#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "CRCharacter.generated.h"

class UGameplayEffect;
class UCRAttributeSet;
class UCRAbilitySystemComponent;

UCLASS()
class COPANDROBBER_API ACRCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Constructor
	ACRCharacter();

	// Ability System Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UCRAbilitySystemComponent* GetCRAbilitySystemComponent() const;

protected:
	
	virtual void BeginPlay() override;
	virtual void ServerSideInit();

	// GAS
	virtual void BindingChangeDelegate();
	void OnSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnStunTagChanged(FGameplayTag Tag, int32 NewCount);
	virtual void OnDeathTagChanged(FGameplayTag Tag, int32 NewCount);

	UPROPERTY(Transient)
	TObjectPtr<UCRAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient)
	TObjectPtr<UCRAttributeSet> AttributeSet;

	// Status & Health
public:
	virtual bool IsDead() const;
	virtual void OnStun();
	virtual void RecoverStun();
	virtual void OnDeath();
	virtual void UpdatedHealth(const FOnAttributeChangeData& OnAttributeChangeData);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DeathEffect;
};
