// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "CRPlayerState.generated.h"

/**
 * 
 */
// 레디 상태가 변경될 때 UI를 업데이트하기 위한 신호
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyStateChange, bool, bIsReady);

UCLASS()
class COPANDROBBER_API ACRPlayerState : public APlayerState, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	
	ACRPlayerState();

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	
	virtual FGenericTeamId GetGenericTeamId() const override;  

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient)
	TObjectPtr<class UCRAttributeSet> AttributeSet;

	UPROPERTY(Replicated)
	FGenericTeamId MyTeamID;
	
};
