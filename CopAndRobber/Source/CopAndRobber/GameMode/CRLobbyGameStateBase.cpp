#include "GameMode/CRLobbyGameStateBase.h"
#include "Net/UnrealNetwork.h"

ACRLobbyGameStateBase::ACRLobbyGameStateBase()
{
	CurrentPlayerCount = 0;
	ReadyPlayerCount   = 0;
}

void ACRLobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACRLobbyGameStateBase, CurrentPlayerCount);
	DOREPLIFETIME(ACRLobbyGameStateBase, ReadyPlayerCount);
}
