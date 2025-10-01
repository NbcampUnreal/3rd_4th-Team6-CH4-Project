#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CRGameInstance.generated.h"

UCLASS()
class COPANDROBBER_API UCRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void SetPlayerNickname(const FString& InNickname) { PlayerNickname = InNickname; }
	FString GetPlayerNickname() const { return PlayerNickname; }

private:
	FString PlayerNickname;
};