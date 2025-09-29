// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CRPlayerResultWidget.h"

#include <GameData/CRPlayerRankInfo.h>

#include "Components/TextBlock.h"
#include "GameMode/CRGameState.h"

void UCRPlayerResultWidget::SetRankInfo(FPlayerRankInfo RankInfo, int32 TotalNum)
{
	CachedTotalNum = TotalNum;
	SetRankText(RankInfo.Rank);
	SetKillText(RankInfo.Kills);
	SetDescriptionText(RankInfo.Rank);
}

void UCRPlayerResultWidget::SetRankText(int32 rank)
{
	if (RankTextBlock)
	{
		FString RankStr = FString::Printf(TEXT("#%d / %d"), rank,CachedTotalNum); 
		RankTextBlock->SetText(FText::FromString(RankStr));
	}
}


void UCRPlayerResultWidget::SetKillText(int32 kill)
{
	if (KillTextBlock)
	{
		FString KillStr = FString::Printf(TEXT("%d킬"), kill); 
		KillTextBlock->SetText(FText::FromString(KillStr));
	}
}

void UCRPlayerResultWidget::SetDescriptionText(int32 rank)
{
	FString Description;
	if (rank == 1)
	{
		Description = TEXT("축하합니다! 1등입니다!");
	}
	else if (rank <= CachedTotalNum / 2) // 상위 절반
	{
		Description = TEXT("아쉽네요.. 그래도 상위권입니다!");
	}
	else 
	{
		Description = TEXT("노력을 하셔야합니다!!!");
	}
	DescriptTextBlock->SetText(FText::FromString(Description));
}
