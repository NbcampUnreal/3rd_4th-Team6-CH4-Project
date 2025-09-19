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
		
		//  ÷̾  MinPlayersToStart ̻ , GamePhase ÷̾    ӽ
		//  غ ư Ȱȭ κ  κп Ͻø ˴ϴ
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

			bool bFoundLocation = NavSys->GetRandomPointInNavigableRadius(SearchOrigin, SearchRadius, RandomLocation);
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


void ACRGameModeBase::BeginGame()
{
	if (CRGameState)
	{
		CRGameState->GamePhase = EGamePhase::GameInProgress;
	}
}