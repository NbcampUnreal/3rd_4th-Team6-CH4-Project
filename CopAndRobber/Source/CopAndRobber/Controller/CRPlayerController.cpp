// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CRPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameData/CRPlayerInputConfig.h"
#include "Controller/CRPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


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

void ACRPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
		{
			FString MapName = GetWorld()->GetMapName();

			if (MapName.Contains(TEXT("TestTitleLevel")))
				{	
				if (TitleWidgetClass)
					{
						TitleWidgetInstance = CreateWidget<UUserWidget>(this, TitleWidgetClass);
						if (TitleWidgetInstance)
						{
							TitleWidgetInstance->AddToViewport();
			
							FInputModeUIOnly InputMode;
							InputMode.SetWidgetToFocus(TitleWidgetInstance->TakeWidget());
							InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
							SetInputMode(InputMode);
							bShowMouseCursor = true;
						}
					}
				}
			else
				{
					if (LobbyWidgetClass)
						{
							LobbyWidgetInstance = CreateWidget<UUserWidget>(this, LobbyWidgetClass);
							if (LobbyWidgetInstance)
							{
								LobbyWidgetInstance->AddToViewport();
							}
						}
					SetInputMode(FInputModeGameOnly());
					bShowMouseCursor = false;
				}
		}
}

void ACRPlayerController::JoinServer(const FString& Address, const FString& Nickname)
{
	FString Options = FString::Printf(TEXT("?Name=%s"), *Nickname);
	UGameplayStatics::OpenLevel(GetWorld(), FName(*Address), true, Options);
}