#include "GameMode/CRLobbyGameModeBase.h"
#include "GameMode/CRPlayerState.h"
#include "GameMode/CRLobbyGameStateBase.h" // ✅ LobbyGameStateBase 포함
#include "GameMode/CRGameInstance.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ACRLobbyGameModeBase::ACRLobbyGameModeBase()
{
	MinPlayersToStart = 2; // 기본값
	GameStateClass = ACRLobbyGameStateBase::StaticClass();
}

void ACRLobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>())
	{
		FString Nickname = PS->GetPlayerName();
		UE_LOG(LogTemp, Log, TEXT("Player joined lobby with nickname: %s"), *Nickname);
	}

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
			}
		}
	}
	
	// GameState에 집계 값 기록 (UI에서 바로 표시 가능)
	LGS->CurrentPlayerCount = LGS->PlayerArray.Num();
	LGS->ReadyPlayerCount   = ReadyCount;

	// 조건 만족 → MainLevel 이동
	if (ReadyCount >= MinPlayersToStart && ReadyCount == LGS->PlayerArray.Num())
	{
		UCRGameInstance* GameInstance = Cast<UCRGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			int32 TotalPlayers = LGS->PlayerArray.Num();
			GameInstance->SetRequiredPlayers(TotalPlayers);
			UE_LOG(LogTemp, Log, TEXT("All %d players ready! Starting game with %d players..."), TotalPlayers, TotalPlayers);
		}

		GetWorld()->ServerTravel(TEXT("/Game/Map/MainLevel"));
	}
}