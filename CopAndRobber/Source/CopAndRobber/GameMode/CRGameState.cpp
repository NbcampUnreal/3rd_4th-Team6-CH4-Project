#include "GameMode/CRGameState.h"
#include "Net/UnrealNetwork.h"

ACRGameState::ACRGameState()
{
	NumPlayers = 0;
	GamePhase = EGamePhase::WaitingForPlayers;
}

void ACRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACRGameState, NumPlayers);
	DOREPLIFETIME(ACRGameState, GamePhase);
}
