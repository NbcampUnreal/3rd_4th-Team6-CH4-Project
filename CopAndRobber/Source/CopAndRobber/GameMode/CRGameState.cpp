#include "GameMode/CRGameState.h"
#include "Net/UnrealNetwork.h"
#include "Gimmick/BlueZone/CRZoneCountdownComponent.h"

ACRGameState::ACRGameState()
{
	NumPlayers = 0;
	GamePhase = EGamePhase::WaitingForPlayers;

	ZoneCountdownComponent = CreateDefaultSubobject<UCRZoneCountdownComponent>(TEXT("ZoneCountdownComponent"));
}

void ACRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACRGameState, NumPlayers);
	DOREPLIFETIME(ACRGameState, GamePhase);
}
