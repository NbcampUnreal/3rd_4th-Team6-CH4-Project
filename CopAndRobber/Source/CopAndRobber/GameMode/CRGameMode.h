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

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	// 자기장 카운트다운을 시작하는 함수
    void StartSafeZoneCountdown();

    // 1초마다 호출되어 타이머를 업데이트하는 함수
    void UpdateSafeZoneTimer();

    // 카운트다운이 종료되었을 때 호출되는 함수
    void OnSafeZoneCountdownFinished();

private:
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	int32 MinPlayersToStart;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<class ACRAISpawner> AISpawnerClass;

	void BeginGame();

	ACRGameState* CRGameState;

	// 타이머를 관리하기 위한 핸들
    FTimerHandle SafeZoneTimerHandle;

    // 자기장 시작 전 카운트다운 시간 (블루프린트에서 수정 가능하도록 설정)
    UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
    int32 SafeZoneCountdownDuration = 60;

    // 서버에서만 사용하는, 현재 남은 시간
    int32 RemainingCountdownTime;
};
