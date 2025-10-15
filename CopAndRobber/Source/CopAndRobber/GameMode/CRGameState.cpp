#include "GameMode/CRGameState.h"
#include "Net/UnrealNetwork.h"
#include "Gimmick/BlueZone/CRBlueZone.h"
#include "Kismet/GameplayStatics.h"

ACRGameState::ACRGameState()
{
	NumPlayers = 0;
	GamePhase = EGamePhase::Waiting;

	if (ACRBlueZone* BlueZone = Cast<ACRBlueZone>(UGameplayStatics::GetActorOfClass(this, ACRBlueZone::StaticClass())))
	{
		CachedShrinkDelay = BlueZone->ShrinkDelay;
	}
	
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
	// �� �Լ��� Ŭ���̾�Ʈ���� PlayerRanks �迭�� ������ ������ ȣ��˴ϴ�.
	// ���⼭ UI ������Ʈ ������ Ʈ������ �� �ֽ��ϴ�.
	// ��������Ʈ���� �� �̺�Ʈ�� �����Ͽ� ������ ������ ������Ʈ�� �� �ֽ��ϴ�.
	UE_LOG(LogTemp, Warning, TEXT("Client: PlayerRanks updated! Current count: %d"), PlayerRanks.Num());

	// ��������Ʈ���� �� �̺�Ʈ�� �����Ͽ� UI�� ������Ʈ�ϴ� ������ �߰��ؾ� �մϴ�.
}

void ACRGameState::OnRep_NumPlayers()
{
	if (GamePhase != EGamePhase::Waiting)
	{
		OnNumPlayersChanged.Broadcast(NumPlayers);
	}
}
