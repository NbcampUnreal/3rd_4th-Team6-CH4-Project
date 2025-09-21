#include "GameMode/CRGameMode.h"
#include "GameMode/CRGameState.h"
#include "GameMode/CRPlayerState.h"
#include "NavigationSystem.h"
#include "Character/Player/CRPlayerCharacter.h"
#include "Controller/CRPlayerController.h"
#include "GameData/CRPlayerInputConfig.h"
#include "AI/AISpawner/CRAISpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"

ACRGameMode::ACRGameMode()
{
	GameStateClass = ACRGameState::StaticClass();
	PlayerStateClass = ACRPlayerState::StaticClass();
	MinPlayersToStart = 2;
}

void ACRGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		FString PlayerName = NewPlayer->GetPlayerState<APlayerState>()->GetPlayerName();
	}

	CRGameState = GetGameState<ACRGameState>();
	if (CRGameState)
	{
		CRGameState->NumPlayers++;
		
		if (CRGameState->NumPlayers >= MinPlayersToStart && CRGameState->GamePhase == EGamePhase::WaitingForPlayers)
		{
			BeginGame();
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

	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		bool bFoundValidLocation = false;
		const int32 MaxAttempts = 10;
		const float SearchRadius = 5000.0f;
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


void ACRGameMode::BeginGame()
{
	if (CRGameState)
	{
		CRGameState->GamePhase = EGamePhase::GameInProgress;
	}

	if (AISpawnerClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ACRAISpawner* AISpawner = World->SpawnActor<ACRAISpawner>(AISpawnerClass, FVector::ZeroVector, FRotator::ZeroRotator);
			if (AISpawner)
			{
				AISpawner->SpawnAllAI();
			}
		}
	}

	// 게임이 시작되면 자기장 카운트다운 시작
	StartSafeZoneCountdown();
}

void ACRGameMode::StartSafeZoneCountdown()
{
    // 서버 로그에 메시지 출력
    UE_LOG(LogTemp, Warning, TEXT("자기장 카운트다운을 시작합니다. (%d초)"), SafeZoneCountdownDuration);

    RemainingCountdownTime = SafeZoneCountdownDuration;

    // GameState를 가져와서 초기 시간 설정
    ACRGameState* CRGameState = GetGameState<ACRGameState>();
    if (CRGameState)
    {
        CRGameState->RemainingTimeBeforeShrink = RemainingCountdownTime;
        // RepNotify가 즉시 호출되지 않으므로 서버 쪽에서도 수동으로 호출해주는 것이 좋음
        CRGameState->OnRep_RemainingTimeBeforeShrink();
    }

    // 1초마다 UpdateSafeZoneTimer 함수를 반복 호출하는 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(
        SafeZoneTimerHandle, 
        this, 
        &ACRGameMode::UpdateSafeZoneTimer, 
        1.0f, 
        true
    );
}

void ACRGameMode::UpdateSafeZoneTimer()
{
    RemainingCountdownTime--;

    // GameState의 시간 업데이트 -> 모든 클라이언트로 복제됨
    ACRGameState* GameState = GetGameState<ACRGameState>();
    if (GameState)
    {
        GameState->RemainingTimeBeforeShrink = RemainingCountdownTime;
        // 서버에서도 RepNotify를 호출하여 일관성 유지
        GameState->OnRep_RemainingTimeBeforeShrink();
    }

    if (RemainingCountdownTime <= 0)
    {
        // 타이머 종료
        GetWorld()->GetTimerManager().ClearTimer(SafeZoneTimerHandle);
        OnSafeZoneCountdownFinished();
    }
}

void ACRGameMode::OnSafeZoneCountdownFinished()
{
    UE_LOG(LogTemp, Warning, TEXT("자기장 카운트다운 종료! 자기장 축소를 시작합니다."));

    // TODO: 여기에 실제 자기장을 줄이는 로직을 시작하는 코드를 추가
}
