#pragma once

#include "CoreMinimal.h"
#include "CRPlayerRankInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerRankInfo
{
	GENERATED_BODY()

	FPlayerRankInfo() : Rank(0), Kills(0), bIsAlive(false) {}

	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly)
	int32 Rank;

	UPROPERTY(BlueprintReadOnly)
	int32 Kills;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAlive;
};
