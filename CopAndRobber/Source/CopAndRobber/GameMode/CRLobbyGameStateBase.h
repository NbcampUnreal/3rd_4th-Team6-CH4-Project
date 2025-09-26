#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CRLobbyGameStateBase.generated.h"

UCLASS()
class COPANDROBBER_API ACRLobbyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACRLobbyGameStateBase();
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Lobby")
	int32 CurrentPlayerCount;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Lobby")
	int32 ReadyPlayerCount;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
