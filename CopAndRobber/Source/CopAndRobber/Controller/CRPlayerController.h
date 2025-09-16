// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CRPlayerController.generated.h"

/**
 * 
 */
class UCRPlayerInputConfig;

UCLASS()
class COPANDROBBER_API ACRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void Client_SetupInput(UCRPlayerInputConfig* InPlayerInputConfig);

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void AcknowledgePossession(class APawn* P) override;
};
