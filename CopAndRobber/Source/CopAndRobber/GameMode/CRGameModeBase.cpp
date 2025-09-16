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
		
		// 현재 플레이어 수가 MinPlayersToStart 이상일 때, GamePhase가 플레이어 대기중 일 때 게임시작
		// 게임 준비 버튼의 활성화 부분은 이 부분에서 수정하시면 됩니다
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

	// Nav 메쉬에 랜덤 위치로 플레이어 생성
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

	// 생성된 플레이어에 Input 설정
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
