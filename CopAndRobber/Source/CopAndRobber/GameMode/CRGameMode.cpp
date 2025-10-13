#include "GameMode/CRGameMode.h"
#include "GameMode/CRGameState.h"
#include "Gimmick/BlueZone/CRZoneCountdownComponent.h"
#include "GameMode/CRPlayerState.h"
#include "GameMode/CRGameInstance.h"
#include "NavigationSystem.h"
#include "Character/Player/CRPlayerCharacter.h"
#include "Controller/CRPlayerController.h"
#include "AI/AISpawner/CRAISpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"

ACRGameMode::ACRGameMode()
{
	GameStateClass = ACRGameState::StaticClass();
	PlayerStateClass = ACRPlayerState::StaticClass();
}

void ACRGameMode::StartPlay()
{
	Super::StartPlay();

	CRGameState = GetGameState<ACRGameState>();
	if (CRGameState)
	{
		CRGameState->GamePhase = EGamePhase::Waiting;
		CRGameState->NumPlayers = 0;
	}


	UCRGameInstance* GameInstance = Cast<UCRGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		RequiredPlayers = GameInstance->GetRequiredPlayers();
	}
}

void ACRGameMode::BeginGame() // player들이 다 접속했을때?
{
	DeadPlayerCount = 0;
	
	CRGameState = GetGameState<ACRGameState>();
	CRGameState->GamePhase = EGamePhase::GameInProgress;
	
	// 점수판 계산 세팅
    CalculateAndSetRanks();

	// 점수판 업데이트 주기 타이머
    GetWorldTimerManager().SetTimer(RankUpdateTimerHandle, this, &ACRGameMode::CalculateAndSetRanks, RankUpdateInterval, true);

	// GameState에 있는 컴포넌트를 통해 자기장 카운트다운 시작
	if (CRGameState->ZoneCountdownComponent)
	{
		CRGameState->ZoneCountdownComponent->ServerStartCountdown();
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in BeginGame. Cannot spawn AI"));
		return;
	}
	
	if (AISpawnerClass)
	{
		{
			ACRAISpawner* AISpawner = World->SpawnActor<ACRAISpawner>(AISpawnerClass, FVector::ZeroVector, FRotator::ZeroRotator);
			if (AISpawner)
			{
				AISpawner->SpawnAllAI();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn ACRAISpawner"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AISpawnerClass is not set in ACRGameMode"));
	}
}

void ACRGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ACRPlayerState* PlayerState = NewPlayer->GetPlayerState<ACRPlayerState>();
	if (CRGameState && CRGameState->GamePhase == EGamePhase::Waiting)
	{
		CRGameState->NumPlayers++;
		
		if (CRGameState->NumPlayers >= RequiredPlayers)
		{
			BeginGame();
			
			for (APlayerState* PS : GameState->PlayerArray)
			{
				if (APlayerController* PC = Cast<APlayerController>(PS->GetOwner()))
				{
					if (!PC->GetPawn())  // 아직 스폰되지 않은 플레이어만
					{
						RestartPlayer(PC);
					}
				}
			}
		}
	}
}

void ACRGameMode::RestartPlayer(AController* NewPlayer)
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

	// 스폰 위치와 회전 초기화
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	// NavMesh 시스템 가져오기
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		bool bFoundValidLocation = false;
		// 유효한 위치를 찾기 위한 최대 시도 횟수
		const int32 MaxAttempts = 10;
		// 검색 반경
		const float SearchRadius = 5000.0f;
		// 검색 원점
		const FVector SearchOrigin = FVector::ZeroVector;

		// TODO: 플레이어 폰의 충돌 크기를 가져와야 합니다. 여기서는 임시로 캡슐 크기를 사용합니다.
		// 실제 프로젝트에서는 DefaultPawn의 CollisionComponent에서 크기를 가져오는 것이 좋습니다.
		const float PlayerCapsuleHalfHeight = 88.0f;
		const float PlayerCapsuleRadius = 34.0f;
		FCollisionShape PlayerShape = FCollisionShape::MakeCapsule(PlayerCapsuleRadius, PlayerCapsuleHalfHeight);

		for (int32 i = 0; i < MaxAttempts; ++i)
		{
			FNavLocation RandomLocation;

			bool bFoundLocation = NavSys->GetRandomReachablePointInRadius(SearchOrigin, SearchRadius, RandomLocation);
			if (bFoundLocation)
			{
				FVector CandidateLocation = RandomLocation.Location;
				
				// 충돌 검사: 해당 위치에 플레이어가 스폰될 공간이 있는지 확인
				bool bHasCollision = GetWorld()->OverlapBlockingTestByChannel(
					CandidateLocation,
					FQuat::Identity,
					ECC_WorldStatic, // 스태틱 메시(건물 등)와의 충돌을 검사
					PlayerShape
				);

				// 충돌이 없으면 유효한 위치로 간주
				if (!bHasCollision)
				{
					SpawnLocation = CandidateLocation + FVector(0.f, 0.f, 50.f); // 바닥에서 살짝 띄워서 스폰
					bFoundValidLocation = true;
					break;
				}
			}
		}

		// 만약 유효한 위치를 찾지 못했다면, 안전한 기본 위치에서 스폰 (예: 0,0,100)
		if (!bFoundValidLocation)
		{
			SpawnLocation = FVector(0.f, 0.f, 100.f);
		}
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;
	SpawnParams.Instigator = NewPlayer->GetPawn();
	// 충돌 시 스폰되지 않도록 핸들링 메소드 변경
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnLocation, SpawnRotation, SpawnParams);

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
	else
	{
		// 스폰에 실패했을 경우 로그 남기기
		UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Failed to spawn pawn for %s at %s"), *NewPlayer->GetName(), *SpawnLocation.ToString());
	}
}

void ACRGameMode::PlayerDied(ACRPlayerState* Player)
{
	if (!CRGameState || CRGameState->GamePhase != EGamePhase::GameInProgress)
	{
		return;
	}

	if (Player)
	{
        Player->SetIsAlive(false);
        DeadPlayerCount++;
        Player->SetDeathOrder(DeadPlayerCount);
        CalculateAndSetRanks();
	}

	int32 TotalPlayerCount = GameState->PlayerArray.Num();
	int32 AlivePlayerCount = TotalPlayerCount - DeadPlayerCount;

	// 생존한 플레이어 숫자가 1명 이하일 경우 게임 종료
	if (AlivePlayerCount <= 1)
	{
		EndGame();
	}
}

void ACRGameMode::EndGame()
{

	CRGameState->GamePhase = EGamePhase::GameFinished;

    GetWorldTimerManager().ClearTimer(RankUpdateTimerHandle);
	
    CalculateAndSetRanks();
	
	// 서버에서 계산한 정확한 순위 정보를 RPC로 직접 전달
	int32 ShowCount = 0;
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (ACRPlayerState* CRPS = Cast<ACRPlayerState>(PS))
		{
			ACRPlayerController* PC = Cast<ACRPlayerController>(CRPS->GetPlayerController());
			if (PC)
			{
				FPlayerRankInfo* FoundRank = CRGameState->PlayerRanks.FindByPredicate([&](const FPlayerRankInfo& Info)
				{
					return Info.PlayerName == CRPS->GetPlayerName();
				});
				if (FoundRank)
				{
					// 서버의 정확한 순위 정보를 RPC로 전달
					PC->Client_ShowResultHUD(*FoundRank, CRGameState->NumPlayers);
					ShowCount++;
				}
			
			}
			
		}
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		UWorld* World = GetWorld();
		if (World)
		{
			UE_LOG(LogTemp, Warning, TEXT("[EndGame] Traveling to Title Level"));
			// 게임이 끝나고 로비 화면으로 돌아가는 로직입니다. 로비 부분을 넣어주면 됩니다
			World->ServerTravel("/Game/Map/TestLobbyLevel", true);
		}
	}, 5.f, false);
}

void ACRGameMode::CalculateAndSetRanks()
{
	if (!CRGameState)
	{
		return;
	}

	TArray<ACRPlayerState*> AllPlayerStates;
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (ACRPlayerState* CRPS = Cast<ACRPlayerState>(PS))
		{
			AllPlayerStates.Add(CRPS);
		}
	}

	// 배틀로얄 순위 계산: 살아있는 사람이 1등, 나중에 죽은 사람이 높은 순위
	// 정렬 순서: 살아있음 > 죽음 (DeathOrder 높은 순)
	AllPlayerStates.Sort([](const ACRPlayerState& A, const ACRPlayerState& B)
	{
		// 1. 생존 여부로 정렬 (살아있는 사람이 먼저)
		if (A.bIsAlive != B.bIsAlive)
		{
			return A.bIsAlive; // true가 먼저 옴
		}

		// 2. 둘 다 살아있다면, 킬 수로 정렬 (킬이 많은 사람이 먼저)
		if (A.bIsAlive)
		{
			if (A.Kills != B.Kills)
			{
				return A.Kills > B.Kills; // 킬 많은 사람이 먼저
			}
			return A.GetPlayerName() < B.GetPlayerName(); // 이름순
		}
		// 3. 둘 다 죽었다면, DeathOrder로 정렬 (나중에 죽은 사람이 먼저)
		else
		{
			if (A.DeathOrder != B.DeathOrder)
			{
				return A.DeathOrder > B.DeathOrder; 
			}
			return A.GetPlayerName() < B.GetPlayerName(); // 이름순
		}
	});
	
	TArray<FPlayerRankInfo> CurrentRanks;
	int32 CurrentRank = 1;
	for (ACRPlayerState* CRPS : AllPlayerStates)
	{
		FPlayerRankInfo RankInfo;
		RankInfo.PlayerName = CRPS->GetPlayerName();
		RankInfo.Rank = CurrentRank++;
		RankInfo.Kills = CRPS->Kills;
		RankInfo.bIsAlive = CRPS->bIsAlive;
		CurrentRanks.Add(RankInfo);
	}

	CRGameState->PlayerRanks = CurrentRanks;
	CRGameState->NumPlayers = GameState->PlayerArray.Num();
}

void ACRGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (!CRGameState)
	{
		return;
	}

	// 게임 진행 중이거나 종료되었으면 로비로 
	if (CRGameState->GamePhase == EGamePhase::GameInProgress ||
		CRGameState->GamePhase == EGamePhase::GameFinished)
	{
		if (ACRPlayerController* PC = Cast<ACRPlayerController>(NewPlayer))
		{
			PC->ClientTravel("/Game/Map/TestLobbyLevel", ETravelType::TRAVEL_Relative);
		}
		return;
	}
	
	if (CRGameState->GamePhase == EGamePhase::Waiting)
	{
	
		if (CRGameState->NumPlayers < RequiredPlayers)
		{
			return;  // 스폰하지 않음
		}
	}
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

