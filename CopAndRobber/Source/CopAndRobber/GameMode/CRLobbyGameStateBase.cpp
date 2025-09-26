#include "CRLobbyGameStateBase.h"
#include "Net/UnrealNetwork.h"

void ACRLobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACRLobbyGameStateBase, LobbyPhase);
}

void ACRLobbyGameStateBase::SetLobbyPhase(ELobbyPhase NewPhase)
{
	if (HasAuthority())
	{
		LobbyPhase = NewPhase;
		OnRep_LobbyPhase(); // 서버에서도 즉시 반영
	}
}

void ACRLobbyGameStateBase::OnRep_LobbyPhase()
{
	UE_LOG(LogTemp, Log, TEXT("[LobbyGameState] Phase changed to: %d"), static_cast<int32>(LobbyPhase));
	// 🔹 여기서 UI 업데이트나 사운드 재생 같은 처리를 해주면 됨
}
