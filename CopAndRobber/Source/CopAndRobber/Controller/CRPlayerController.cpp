// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CRPlayerController.h"

#include "Character/Player/CRPlayerCharacter.h"

void ACRPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


}

void ACRPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
}

