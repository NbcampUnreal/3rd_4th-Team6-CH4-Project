// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CRCharacter.h"
#include "GAS/Ability/ECRAbilityInputID.h"
#include "GenericTeamAgentInterface.h"
#include "CRPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UCRPlayerInputConfig;
class UInputAction;
/**
 * 
 */
UCLASS()
class COPANDROBBER_API ACRPlayerCharacter : public ACRCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACRPlayerCharacter();
	
	UFUNCTION()
	void OnEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
	void OnGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect);
	void SendInstigatorEvent();
	
	UFUNCTION(Server, Reliable)
	void ServerInteractDoor(ACRDoor* Door);
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void BindingChangeDelegate() override;
	void OnEnemyKilled(const FGameplayEventData* Payload);
	
	virtual void OnStun() override;
	virtual void UpdatedHealth(const FOnAttributeChangeData& OnAttributeChangeData) override;

	virtual void RecoverStun() override;
	virtual void OnDeath()override;


#pragma region Camera Setting
protected:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float TargetArmLength;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FVector SocketOffset;

#pragma endregion

#pragma  region Input
protected:
	void HandleMoveAction(const FInputActionValue& Value);
	void HandleLookAction(const FInputActionValue& Value);
	void HandleReadyAction(const FInputActionValue& Value);
	void HandleInteractAction(const FInputActionValue& Value);
	
	void HandleAbilityPressedAction(const FInputActionValue& Value, ECRAbilityInputID Key);
	void HandleAbilityReleaseAction(const FInputActionValue& Value, ECRAbilityInputID Key);
public:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UCRPlayerInputConfig> PlayerInputConfig;

	
#pragma endregion


#pragma region TeamID
public:
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID);
	
	virtual FGenericTeamId GetGenericTeamId() const ;

protected:
	UPROPERTY(EditAnywhere, Category= "Team")
	FGenericTeamId TeamId;
	
#pragma endregion
public:
	void SetLastDamageInstigator(AActor* DamageInstigator) { LastDamageInstigator = DamageInstigator; };
private:
	AActor* LastDamageInstigator;
};
