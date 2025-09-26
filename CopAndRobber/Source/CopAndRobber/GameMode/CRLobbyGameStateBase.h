#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CRLobbyGameStateBase.generated.h"

UENUM(BlueprintType)
enum class ELobbyPhase : uint8
{
	WaitingForPlayers UMETA(DisplayName="Waiting For Players"), //기본 상태
	AllReady          UMETA(DisplayName="All Ready"), // 모두 준비
	Starting          UMETA(DisplayName="Starting"), // 시작
};

UCLASS()
class COPANDROBBER_API ACRLobbyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	// 현재 로비 상태
	UPROPERTY(ReplicatedUsing=OnRep_LobbyPhase, BlueprintReadOnly, Category="Lobby")
	ELobbyPhase LobbyPhase = ELobbyPhase::WaitingForPlayers;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetLobbyPhase(ELobbyPhase NewPhase);

protected:
	UFUNCTION()
	void OnRep_LobbyPhase();
};
