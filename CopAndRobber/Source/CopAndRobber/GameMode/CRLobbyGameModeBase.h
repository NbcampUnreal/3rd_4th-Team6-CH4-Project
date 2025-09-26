#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CRLobbyGameModeBase.generated.h"

UCLASS()
class COPANDROBBER_API ACRLobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACRLobbyGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void CheckAllPlayersReady();

protected:
	// ✅ 최소 인원 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lobby Rules")
	int32 MinPlayersToStart;
};
