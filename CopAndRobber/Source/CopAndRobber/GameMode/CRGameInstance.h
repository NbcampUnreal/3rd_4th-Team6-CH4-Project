#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CRGameInstance.generated.h"

UCLASS()
class COPANDROBBER_API UCRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 닉네임 관련
	void SetPlayerNickname(const FString& InNickname) { PlayerNickname = InNickname; }
	FString GetPlayerNickname() const { return PlayerNickname; }

	// 로비에서
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetRequiredPlayers(int32 InRequiredPlayers) { RequiredPlayers = InRequiredPlayers; }

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	int32 GetRequiredPlayers() const { return RequiredPlayers; }


private:
	FString PlayerNickname;
	
	int32 RequiredPlayers = 2;
	
};