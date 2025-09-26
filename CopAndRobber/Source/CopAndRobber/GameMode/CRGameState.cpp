#include "GameMode/CRGameState.h"
#include "Net/UnrealNetwork.h"
#include "Gimmick/BlueZone/CRZoneCountdownComponent.h"

ACRGameState::ACRGameState()
{
	NumPlayers = 0;
	GamePhase = EGamePhase::Waiting;

	ZoneCountdownComponent = CreateDefaultSubobject<UCRZoneCountdownComponent>(TEXT("ZoneCountdownComponent"));
}

void ACRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACRGameState, NumPlayers);
	DOREPLIFETIME(ACRGameState, GamePhase);
	DOREPLIFETIME(ACRGameState, PlayerRanks);
}

void ACRGameState::OnRep_PlayerRanks()
{
	// 이 함수는 클라이언트에서 PlayerRanks 배열이 복제될 때마다 호출됩니다.
	// 여기서 UI 업데이트 로직을 트리거할 수 있습니다.
	// 블루프린트에서 이 이벤트를 수신하여 점수판 위젯을 업데이트할 수 있습니다.
	UE_LOG(LogTemp, Warning, TEXT("Client: PlayerRanks updated! Current count: %d"), PlayerRanks.Num());

	// 블루프린트에서 이 이벤트를 수신하여 UI를 업데이트하는 로직을 추가해야 합니다.
}
