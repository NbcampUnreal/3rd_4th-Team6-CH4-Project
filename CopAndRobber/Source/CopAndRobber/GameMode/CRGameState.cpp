#include "GameMode/CRGameState.h"
#include "Net/UnrealNetwork.h"

ACRGameState::ACRGameState()
{
	NumPlayers = 0;
	GamePhase = EGamePhase::WaitingForPlayers;
	RemainingTimeBeforeShrink = 0; // 초기값 설정
}

void ACRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACRGameState, NumPlayers);
	DOREPLIFETIME(ACRGameState, GamePhase);
	// RemainingTimeBeforeShrink 변수를 모든 클라이언트에 복제하도록 설정
	DOREPLIFETIME(ACRGameState, RemainingTimeBeforeShrink);
}

void ACRGameState::OnRep_RemainingTimeBeforeShrink()
{
    // 남은 시간이 변경되었음을 UI에 알림 (델리게이트 호출)
    OnRemainingTimeChanged.Broadcast(RemainingTimeBeforeShrink);
}