#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CRGameMode.generated.h"

class ACRGameState;

UCLASS()
class COPANDROBBER_API ACRGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACRGameMode();

	void CheckAllPlayersReady();
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	int32 MinPlayersToStart;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<class ACRAISpawner> AISpawnerClass;

	void BeginGame();

	ACRGameState* CRGameState;
};