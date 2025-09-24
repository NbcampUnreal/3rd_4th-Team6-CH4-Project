#include "CRLobbyPlayerEntry.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/CRPlayerState.h"

void UCRLobbyPlayerEntry::Init(APlayerState* PlayerState)
{
	if (!PlayerState) return;

	FString Nickname = PlayerState->GetPlayerName();
	bool bIsReady = false;

	if (ACRPlayerState* CRState = Cast<ACRPlayerState>(PlayerState))
	{
		bIsReady = CRState->bIsReady;
	}

	if (TextBlock_Nickname)
	{
		TextBlock_Nickname->SetText(FText::FromString(Nickname));
	}

	if (TextBlock_ReadyState)
	{
		TextBlock_ReadyState->SetText(
			bIsReady ? FText::FromString(TEXT("준비됨")) : FText::FromString(TEXT("대기 중"))
		);
	}
}
