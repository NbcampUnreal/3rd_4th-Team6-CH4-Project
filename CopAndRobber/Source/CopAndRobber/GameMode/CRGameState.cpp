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
	// �� �Լ��� Ŭ���̾�Ʈ���� PlayerRanks �迭�� ������ ������ ȣ��˴ϴ�.
	// ���⼭ UI ������Ʈ ������ Ʈ������ �� �ֽ��ϴ�.
	// ��������Ʈ���� �� �̺�Ʈ�� �����Ͽ� ������ ������ ������Ʈ�� �� �ֽ��ϴ�.
	UE_LOG(LogTemp, Warning, TEXT("Client: PlayerRanks updated! Current count: %d"), PlayerRanks.Num());

	// ��������Ʈ���� �� �̺�Ʈ�� �����Ͽ� UI�� ������Ʈ�ϴ� ������ �߰��ؾ� �մϴ�.
}
