#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameData/CRPlayerRankInfo.h"
#include "CRGameState.generated.h"

class UCRZoneCountdownComponent;

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Waiting,			// 대기중
	GameInProgress,		// 게임 진행중
	GameFinished		// 게임 종료
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNumPlayersChanged, int32, NewNumPlayers);
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
	
	UPROPERTY(ReplicatedUsing= OnRep_NumPlayers, BlueprintReadOnly, Category = "GameState")
	int32 NumPlayers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	EGamePhase GamePhase;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerRanks, BlueprintReadOnly, Category = "GameState")
	TArray<FPlayerRankInfo> PlayerRanks;

	UPROPERTY(BlueprintAssignable, Category = "GameState|Events")
	FOnNumPlayersChanged OnNumPlayersChanged;

protected:
	// 변수 복제를 위해 필수적으로 오버라이드해야 하는 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// OnRep_PlayerRanks 함수 선언
	UFUNCTION()
	void OnRep_PlayerRanks();

	UFUNCTION()
	void OnRep_NumPlayers();
};
