#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CRGameMode.generated.h"

class ACRGameState;
class ACRPlayerState;

UCLASS()
class COPANDROBBER_API ACRGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACRGameMode();
	void PlayerDied(ACRPlayerState* Player);
	
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void StartPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<class ACRAISpawner> AISpawnerClass;

	UPROPERTY()
	ACRGameState* CRGameState;

	// 로비에서 설정한 게임 시작에 필요한 플레이어 수 (GameInstance에서 가져옴)
	int32 RequiredPlayers = 4;
	
	void BeginGame();
	void EndGame();

	// New: Function to calculate and set ranks in GameState
	void CalculateAndSetRanks();

	// Optional: Timer handle for periodic rank updates
	FTimerHandle RankUpdateTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Ranking")
	float RankUpdateInterval = 5.0f; // Update ranks every 5 seconds

	UPROPERTY()
	int32 DeadPlayerCount = 0;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};