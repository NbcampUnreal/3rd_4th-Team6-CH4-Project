#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CRGameState.generated.h"

// UI 업데이트를 위해 델리게이트 선언 (블루프린트에서도 사용 가능)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemainingTimeChanged, int32, NewTime);

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
	
	// 자기장 시작까지 남은 시간 (초)
    // ReplicatedUsing: 이 변수가 클라이언트에서 변경될 때마다 OnRep_RemainingTimeBeforeShrink 함수가 호출됨
    UPROPERTY(ReplicatedUsing = OnRep_RemainingTimeBeforeShrink)
    int32 RemainingTimeBeforeShrink;

    // UI 바인딩을 위한 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Game")
    FOnRemainingTimeChanged OnRemainingTimeChanged;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	int32 NumPlayers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	EGamePhase GamePhase;

protected:
	// 변수 복제를 위해 필수적으로 오버라이드해야 하는 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// RemainingTimeBeforeShrink 변수가 복제될 때 클라이언트에서 호출될 함수
    UFUNCTION()
    void OnRep_RemainingTimeBeforeShrink();
};