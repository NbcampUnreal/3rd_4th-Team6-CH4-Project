#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CRGameState.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	WaitingForPlayers,	// 플레이어 대기중
	GameInProgress,		// 게임 진행중
	GameFinished		// 게임 종료
};

/**
 * 
 */
UCLASS()
class COPANDROBBER_API ACRGameState : public AGameState
{
	GENERATED_BODY()

public:
	ACRGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	int32 NumPlayers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	EGamePhase GamePhase;
};
