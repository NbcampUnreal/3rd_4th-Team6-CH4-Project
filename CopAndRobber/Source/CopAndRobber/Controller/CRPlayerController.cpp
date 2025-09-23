// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CRPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameData/CRPlayerInputConfig.h"
#include "Controller/CRPlayerController.h"

#include "GameplayEffect.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameMode/CRGameState.h"
#include "Gimmick/BlueZone/CRZoneCountdownComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CRBattleHUDWidget.h"


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

	ShowBattleHUD();
}

void ACRPlayerController::ShowBattleHUD()
{
	if (BattleWidgetClass && IsLocalController())
	{
		BattleWidgetInstance = CreateWidget<UCRBattleHUDWidget>(this, BattleWidgetClass);
		if (BattleWidgetInstance)
		{
			BattleWidgetInstance->AddToViewport();
			BindingBattleHUD();
		}
	}
}

void ACRPlayerController::BindingBattleHUD()
{
	if (ACRGameState* GS = Cast<ACRGameState>(GetWorld()-> GetGameState()))
	{
		
		GS->ZoneCountdownComponent->OnZoneCountingChanged.RemoveDynamic(
			BattleWidgetInstance, 
			&UCRBattleHUDWidget::SetUpRemainingTextBlock
		);
    
		GS->ZoneCountdownComponent->OnZoneTimeChanged.RemoveDynamic(
			BattleWidgetInstance, 
			&UCRBattleHUDWidget::SetTimerRemaining
		);
		
		GS->ZoneCountdownComponent->OnZoneCountingChanged.AddDynamic(
			BattleWidgetInstance, 
			&UCRBattleHUDWidget::SetUpRemainingTextBlock
		);
		
		GS->ZoneCountdownComponent->OnZoneTimeChanged.AddDynamic(
			BattleWidgetInstance, 
			&UCRBattleHUDWidget::SetTimerRemaining
		);
	}
}

void ACRPlayerController::JoinServer(const FString& Address, const FString& Nickname)
{
	FString Options = FString::Printf(TEXT("?Name=%s"), *Nickname);
	UGameplayStatics::OpenLevel(GetWorld(), FName(*Address), true, Options);
}

void ACRPlayerController::UpdateBuffUI(const FGameplayTag& Tag, int Count)
{
	if (BattleWidgetInstance)
	{
		BattleWidgetInstance->UpdateBuffSlot(Tag, Count);
	}
}

void ACRPlayerController::RemoveBuffUI(const FGameplayTag& Tag)
{
	if (BattleWidgetInstance)
	{
		BattleWidgetInstance->RemoveBuffSlot(Tag);
	}
}

