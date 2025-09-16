// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CRPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameData/CRPlayerInputConfig.h"

void ACRPlayerController::Client_SetupInput_Implementation(UCRPlayerInputConfig* InPlayerInputConfig)
{
	if (InPlayerInputConfig == nullptr)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InPlayerInputConfig->DefaultMappingContext)
		{
			Subsystem->AddMappingContext(InPlayerInputConfig->DefaultMappingContext, 0);
		}
	}
}

void ACRPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACRPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
}

