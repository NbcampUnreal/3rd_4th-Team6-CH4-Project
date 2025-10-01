// Fill out your copyright notice in the Description page of Project Settings.

#include "Controller/CRPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameData/CRPlayerInputConfig.h"

#include "GameplayEffect.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "GameMode/CRGameState.h"
#include "Gimmick/BlueZone/CRZoneCountdownComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CRBattleHUDWidget.h"
#include "UI/CRLobbyWidget.h"
#include "UI/CRPlayerResultWidget.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/CRGameInstance.h"
#include "GameMode/CRGameMode.h"
#include "GameMode/CRLobbyGameModeBase.h"
#include "GameMode/CRPlayerState.h"

void ACRPlayerController::Client_SetupInput_Implementation(UCRPlayerInputConfig* InPlayerInputConfig)
{
	if (!InPlayerInputConfig)
		return;
	
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

	if (!HasAuthority())
	{
		UCRGameInstance* CRGI = Cast<UCRGameInstance>(GetGameInstance());
		if (CRGI)
		{
			const FString Nickname = CRGI->GetPlayerNickname();
			if (!Nickname.IsEmpty())
			{
				Server_SetNickname(Nickname);
			}
		}
	}
	
	if (!IsLocalController())
		return;

	FString MapName = GetWorld()->GetMapName();

	// 로비 제거 처리
	if (!MapName.Contains(TEXT("TestLobbyLevel")) && LobbyWidgetInstance)
	{
		LobbyWidgetInstance->RemoveFromParent();
		LobbyWidgetInstance = nullptr;
	}

	// 타이틀 UI
	if (MapName.Contains(TEXT("TestTitleLevel")))
	{
		if (TitleWidgetClass)
		{
			TitleWidgetInstance = CreateWidget<UUserWidget>(this, TitleWidgetClass);
			if (TitleWidgetInstance)
			{
				TitleWidgetInstance->SetIsFocusable(true);
				TitleWidgetInstance->AddToViewport();

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(TitleWidgetInstance->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				SetInputMode(InputMode);
				bShowMouseCursor = true;
			}
		}
	}
	// 로비 UI
	else if (MapName.Contains(TEXT("TestLobbyLevel")))
	{
		if (LobbyWidgetClass)
		{
			LobbyWidgetInstance = CreateWidget<UCRLobbyWidget>(this, LobbyWidgetClass);
			if (LobbyWidgetInstance)
			{
				LobbyWidgetInstance->AddToViewport();
				LobbyWidgetInstance->RefreshPlayerList();
			}
		}
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else if (MapName.Contains(TEXT("MainLevel")))
	{
		ShowBattleHUD();
	}
	
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

void ACRPlayerController::ShowResultHUD(const FPlayerRankInfo& RankInfo, int32 TotalPlayers)
{
	if (ResultWidgetClass && IsLocalController())
	{
		ResultWidgetInstance = CreateWidget<UCRPlayerResultWidget>(this, ResultWidgetClass);
		if (ResultWidgetInstance)
		{
			ResultWidgetInstance->AddToViewport();
			ResultWidgetInstance->SetRankInfo(RankInfo, TotalPlayers);

			SetInputMode(FInputModeUIOnly());
			SetShowMouseCursor(true);
		}
	
	}
}

void ACRPlayerController::Client_ShowResultHUD_Implementation(FPlayerRankInfo RankInfo, int32 TotalPlayers)
{
	ShowResultHUD(RankInfo, TotalPlayers);
}

void ACRPlayerController::BindingBattleHUD()
{
	if (ACRGameState* GS = Cast<ACRGameState>(GetWorld()->GetGameState()))
	{
		if (BattleWidgetInstance)
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

	// PendingNickname 처리
	if (!HasAuthority() && !PendingNickname.IsEmpty())
	{
		Server_SetNickname(PendingNickname);
	}
}

void ACRPlayerController::JoinServer(const FString& Address, const FString& Nickname)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*Address), true);
}


void ACRPlayerController::Server_SetNickname_Implementation(const FString& Nickname)
{
	if (ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>())
	{
		CRPS->Server_SetNickname(Nickname); 
		CRPS->SetPlayerName(Nickname); 
		UE_LOG(LogTemp, Log, TEXT("서버에 닉네임 설정됨: %s"), *Nickname);
	}
}



void ACRPlayerController::Server_ToggleReady_Implementation()
{
	if (ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>())
	{
		CRPS->bIsReady = !CRPS->bIsReady;

		UE_LOG(LogTemp, Warning, TEXT("[SERVER] %s is now %s"),
			*CRPS->GetPlayerName(),
			CRPS->bIsReady ? TEXT("READY") : TEXT("NOT READY"));

		// ✅ GameMode 호출
		if (ACRLobbyGameModeBase* GM = GetWorld()->GetAuthGameMode<ACRLobbyGameModeBase>())
		{
			GM->CheckAllPlayersReady();
		}
	}
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

void ACRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// ESC 키 입력 바인딩
	InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &ACRPlayerController::HandleEscapePressed);
}

void ACRPlayerController::HandleEscapePressed()
{
	FString MapName = GetWorld()->GetMapName();

	UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent called")); // 로그 찍어보기
	// ✅ 로비에서만 ESC 허용
	if (!MapName.Contains(TEXT("TestLobbyLevel")))
	{
		return;
	}

	if (!GetWorld()->GetMapName().Contains(TEXT("TestLobbyLevel")))
		return;
	
	if (EscapeMenuInstance && EscapeMenuInstance->IsInViewport())
	{
		// ESC 메뉴 닫기
		EscapeMenuInstance->RemoveFromParent();
		EscapeMenuInstance = nullptr;

		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else if (EscapeMenuClass)
	{
		// ESC 메뉴 열기
		EscapeMenuInstance = CreateWidget<UUserWidget>(this, EscapeMenuClass);
		if (EscapeMenuInstance)
		{
			EscapeMenuInstance->AddToViewport();

			FInputModeGameAndUI InputMode;
			InputMode.SetHideCursorDuringCapture(false);
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);
			bShowMouseCursor = true;

		}
	}
}
