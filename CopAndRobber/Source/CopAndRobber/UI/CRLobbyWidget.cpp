// CRLobbyWidget.cpp

#include "CRLobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "UI/CRLobbyPlayerEntry.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void UCRLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(
		RefreshTimerHandle,
		this,
		&UCRLobbyWidget::RefreshPlayerList,
		0.2f, // 0.2초 간격
		true  // 반복
	);

	// 2초 후에 타이머 종료
	FTimerHandle StopTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		StopTimerHandle,
		[this]()
		{
			GetWorld()->GetTimerManager().ClearTimer(RefreshTimerHandle);
		},
		2.0f, // 총 2초 동안만 반복
		false
	);
}


void UCRLobbyWidget::RefreshPlayerList()
{
	if (!ScrollBox_PlayerList)
	{
		return;
	}

	if (!LobbyPlayerEntryClass)
	{
		return;
	}

	ScrollBox_PlayerList->ClearChildren();

	AGameStateBase* GameState = GetWorld()->GetGameState();
	if (!GameState)
	{
		return;
	}

	const TArray<APlayerState*>& Players = GameState->PlayerArray;
	

	if (TextBlock_PlayerCount)
	{
		TextBlock_PlayerCount->SetText(FText::FromString(
			FString::Printf(TEXT("접속 인원: %d명"), Players.Num())
		));
	}

	for (APlayerState* PlayerState : Players)
	{
		UCRLobbyPlayerEntry* PlayerEntry = CreateWidget<UCRLobbyPlayerEntry>(this, LobbyPlayerEntryClass);
		if (!PlayerEntry)
		{
			continue;
		}
		PlayerEntry->Init(PlayerState);
		ScrollBox_PlayerList->AddChild(PlayerEntry);
	}
}
