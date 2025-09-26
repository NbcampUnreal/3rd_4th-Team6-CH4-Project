#include "GameMode/CRLobbyGameModeBase.h"
#include "GameMode/CRPlayerState.h"
#include "GameMode/CRLobbyGameStateBase.h" // ✅ LobbyGameStateBase 포함
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"

ACRLobbyGameModeBase::ACRLobbyGameModeBase()
{
	MinPlayersToStart = 2; // 기본값
}

void ACRLobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("Player %s joined lobby"), *NewPlayer->GetName());

	CheckAllPlayersReady();
}

void ACRLobbyGameModeBase::CheckAllPlayersReady()
{
	if (!GameState) return;

	int32 ReadyCount = 0;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (ACRPlayerState* CRPS = Cast<ACRPlayerState>(PS))
		{
			if (CRPS->bIsReady)
			{
				ReadyCount++;
			}
		}
	}

	ACRLobbyGameStateBase* LGS = GetGameState<ACRLobbyGameStateBase>();
	if (!LGS) return;

	if (ReadyCount >= MinPlayersToStart && ReadyCount == GameState->PlayerArray.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("All players ready!"));

		// ✅ GameState Phase 갱신
		LGS->SetLobbyPhase(ELobbyPhase::AllReady);

		// 필요하다면 카운트다운 같은 걸 거치고 → Start 상태로 전환
		LGS->SetLobbyPhase(ELobbyPhase::Starting);

		// ✅ 게임 시작
		GetWorld()->ServerTravel(TEXT("/Game/Map/MainLevel?listen"));
	}
	else
	{
		// 아직 준비 안 됨
		LGS->SetLobbyPhase(ELobbyPhase::WaitingForPlayers);

		UE_LOG(LogTemp, Log, TEXT("Ready players: %d / %d (Need at least %d)"),
			ReadyCount, GameState->PlayerArray.Num(), MinPlayersToStart);
	}
}
