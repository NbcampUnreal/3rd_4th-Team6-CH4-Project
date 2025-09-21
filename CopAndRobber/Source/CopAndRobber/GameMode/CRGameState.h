#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CRGameState.generated.h"

class UCRZoneCountdownComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCRZoneCountdownComponent* ZoneCountdownComponent;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	int32 NumPlayers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	EGamePhase GamePhase;

protected:
	// 변수 복제를 위해 필수적으로 오버라이드해야 하는 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
