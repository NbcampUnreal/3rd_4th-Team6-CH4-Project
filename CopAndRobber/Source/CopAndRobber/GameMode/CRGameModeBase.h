// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CRGameModeBase.generated.h"

class ACRGameState;

UCLASS()
class COPANDROBBER_API ACRGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	ACRGameModeBase();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	int32 MinPlayersToStart;

	void BeginGame();

	ACRGameState* CRGameState;
};