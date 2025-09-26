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
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	int32 MinPlayersToStart;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<class ACRAISpawner> AISpawnerClass;

	UPROPERTY()
	ACRGameState* CRGameState;
	
	void BeginGame();
	void EndGame();

	// New: Function to calculate and set ranks in GameState
	void CalculateAndSetRanks();

	// Optional: Timer handle for periodic rank updates
	FTimerHandle RankUpdateTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Ranking")
	float RankUpdateInterval = 5.0f; // Update ranks every 5 seconds
};