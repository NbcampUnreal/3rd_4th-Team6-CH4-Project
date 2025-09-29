#include "GameMode/CRLobbyGameModeBase.h"
#include "GameMode/CRPlayerState.h"
#include "GameMode/CRLobbyGameStateBase.h" // ✅ LobbyGameStateBase 포함
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"

ACRLobbyGameModeBase::ACRLobbyGameModeBase()
{
	MinPlayersToStart = 2; // 기본값
	GameStateClass = ACRLobbyGameStateBase::StaticClass();
}

void ACRLobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("Player %s joined lobby"), *NewPlayer->GetName());

	CheckAllPlayersReady();
}

void ACRLobbyGameModeBase::CheckAllPlayersReady()
{
	ACRLobbyGameStateBase* LGS = GetGameState<ACRLobbyGameStateBase>();
	if (!LGS) return;

	int32 ReadyCount = 0;
	for (APlayerState* PS : LGS->PlayerArray)
	{
		if (ACRPlayerState* CRPS = Cast<ACRPlayerState>(PS))
		{
			if (CRPS->bIsReady)
			{
				ReadyCount++;
				UE_LOG(LogTemp, Log, TEXT("%d"), ReadyCount);
			}
		}
	}
	
	// GameState에 집계 값 기록 (UI에서 바로 표시 가능)
	LGS->CurrentPlayerCount = LGS->PlayerArray.Num();
	LGS->ReadyPlayerCount   = ReadyCount;
	// 조건 만족 → MainLevel 이동
	if (ReadyCount >= MinPlayersToStart && ReadyCount == LGS->PlayerArray.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("All players ready! Moving to MainLevel..."));
		GetWorld()->ServerTravel(TEXT("/Game/Map/MainLevel?listen"));
	}
}