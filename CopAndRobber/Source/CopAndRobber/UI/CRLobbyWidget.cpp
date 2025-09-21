#include "UI/CRLobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameMode/CRPlayerState.h"
#include "Controller/CRPlayerController.h"
#include "GameFramework/PlayerController.h"

void UCRLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReadyButton)
	{
		ReadyButton->OnClicked.AddDynamic(this, &UCRLobbyWidget::OnClickReady);
	}

	// 1초 주기 갱신
	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().SetTimer(RefreshTimer, this, &UCRLobbyWidget::RefreshPlayerCount, 1.0f, true);
	}

	RefreshPlayerCount();
	RefreshReadyText();
}

void UCRLobbyWidget::NativeDestruct()
{
	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(RefreshTimer);
	}
	Super::NativeDestruct();
}

void UCRLobbyWidget::OnClickReady()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		//if (ACRPlayerState* PS = PC->GetPlayerState<ACRPlayerState>())
		//{
			//const bool bNew = !PS->IsReady();
			// 서버로 내 Ready 상태 전송
			//if (ACRPlayerController* CRPC = Cast<ACRPlayerController>(PC))
			//{
			//	CRPC->ServerSetReady(bNew);
			//}
		//}
	}
}


void UCRLobbyWidget::RefreshPlayerCount()
{
	int32 Count = 1;
	if (UWorld* W = GetWorld())
	{
		if (AGameStateBase* GS = W->GetGameState())
		{
			Count = GS->PlayerArray.Num();
		}
	}
	if (PlayerCountText)
	{
		PlayerCountText->SetText(FText::AsNumber(Count));
	}
	RefreshReadyText();
}

void UCRLobbyWidget::RefreshReadyText()
{
	FText T = FText::FromString(TEXT("Not Ready"));
	if (APlayerController* PC = GetOwningPlayer())
	{
		//if (ACRPlayerState* PS = PC->GetPlayerState<ACRPlayerState>())
		//{
		//	T = PS->IsReady() ? FText::FromString(TEXT("Ready")) : FText::FromString(TEXT("Not Ready"));
		//}
	}
	if (ReadyStateText) ReadyStateText->SetText(T);
}

