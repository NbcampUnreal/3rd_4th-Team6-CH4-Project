#include "CRLobbyPlayerEntry.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/CRPlayerState.h"

void UCRLobbyPlayerEntry::Init(APlayerState* PlayerState)
{
	if (!PlayerState) return;
	
	bool bIsReady = false;
	FString Nickname = TEXT("알 수 없음");

	if (ACRPlayerState* CRState = Cast<ACRPlayerState>(PlayerState))
	{
		Nickname = CRState->GetNickname(); // ✅ 커스텀 닉네임 사용
		bIsReady = CRState->bIsReady;
	}
	else
	{
		// Fallback (안정성용)
		Nickname = PlayerState->GetPlayerName();
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
