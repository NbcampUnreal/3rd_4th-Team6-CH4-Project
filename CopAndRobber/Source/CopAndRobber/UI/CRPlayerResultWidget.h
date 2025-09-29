// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRPlayerResultWidget.generated.h"

struct FPlayerRankInfo;
class UTextBlock;
/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRPlayerResultWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetRankInfo(FPlayerRankInfo ACRPlayerState, int32 TotalNum);

	void SetRankText(int32 rank);

	void SetKillText(int32 kill);

	void SetDescriptionText(int32 rank);

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RankTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> KillTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DescriptTextBlock;

	int32 CachedTotalNum = 0;
	
	
};
