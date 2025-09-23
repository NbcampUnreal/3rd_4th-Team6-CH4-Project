// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CRBattleHUDWidget.h"

#include "Components/TextBlock.h"


void UCRBattleHUDWidget::SetUpRemainingTextBlock(bool bIsCounting)
{
	if (bIsCounting)
	{
		TimerTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TimerTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCRBattleHUDWidget::SetTimerRemaining(int32 RemainingTime)
{
	RemainingTime = FMath::Max(0, RemainingTime);
    
	int32 Minutes = RemainingTime / 60;
	int32 Seconds = RemainingTime % 60;

	FString TimeString = FString::Printf(TEXT("자기장 축소 %02d:%02d"), Minutes, Seconds);
	FText TimeText = FText::FromString(TimeString);
	TimerTextBlock->SetText(TimeText);
}

void UCRBattleHUDWidget::UpdateAliveCountTextBlock(int32 AliveCount)
{
	FString Formatted = FString::Printf(TEXT("남은 인원 : %d"), AliveCount);
	AliveCountTextBlock->SetText(FText::FromString(Formatted));
}

