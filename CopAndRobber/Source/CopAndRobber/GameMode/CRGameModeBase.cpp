#include "GameMode/CRGameModeBase.h"
#include "GameMode/CRGameState.h"
#include "GameMode/CRPlayerState.h"
#include "NavigationSystem.h"
#include "Character/Player/CRPlayerCharacter.h"
#include "Controller/CRPlayerController.h"
#include "GameData/CRPlayerInputConfig.h"

ACRGameModeBase::ACRGameModeBase()
{
	GameStateClass = ACRGameState::StaticClass();
	PlayerStateClass = ACRPlayerState::StaticClass();
	MinPlayersToStart = 2;
}

void ACRGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CRGameState = GetGameState<ACRGameState>();
	if (CRGameState)
	{
		CRGameState->NumPlayers++;
		
		// ���� �÷��̾� ���� MinPlayersToStart �̻��� ��, GamePhase�� �÷��̾� ����� �� �� ���ӽ���
		// ���� �غ� ��ư�� Ȱ��ȭ �κ��� �� �κп��� �����Ͻø� �˴ϴ�
		if (CRGameState->NumPlayers >= MinPlayersToStart && CRGameState->GamePhase == EGamePhase::WaitingForPlayers)
		{
			BeginGame();
		}
	}
}

void ACRGameModeBase::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	TSubclassOf<APawn> PawnClass = GetDefaultPawnClassForController(NewPlayer);
	if (PawnClass == nullptr)
	{
		return;
	}

	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// Nav �޽��� ���� ��ġ�� �÷��̾� ����
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		FNavLocation RandomLocation;
		const float SearchRadius = 5000.0f;
		bool bFoundLocation = NavSys->GetRandomPointInNavigableRadius(FVector::ZeroVector, SearchRadius, RandomLocation);
		if (bFoundLocation)
		{
			SpawnLocation = RandomLocation.Location + FVector(0.f, 0.f, 100.f);
		}
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;
	SpawnParams.Instigator = NewPlayer->GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnLocation, SpawnRotation, SpawnParams);

	// ������ �÷��̾ Input ����
	if (NewPawn)
	{
		NewPlayer->Possess(NewPawn);
		
		ACRPlayerController* PC = Cast<ACRPlayerController>(NewPlayer);
		ACRPlayerCharacter* PCH = Cast<ACRPlayerCharacter>(NewPawn);
		if (PC && PCH)
		{
			PC->Client_SetupInput(PCH->PlayerInputConfig);
		}
	}
}


void ACRGameModeBase::BeginGame()
{
	if (CRGameState)
	{
		CRGameState->GamePhase = EGamePhase::GameInProgress;
	}
}
