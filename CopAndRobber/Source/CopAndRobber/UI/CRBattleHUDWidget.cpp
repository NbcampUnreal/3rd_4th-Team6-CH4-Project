// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CRBattleHUDWidget.h"

#include "Components/TextBlock.h"

void UCRBattleHUDWidget::InitializeSetupHUD(int32 SetTime, int32 SetAliveCount)
{
	CachedRemainingTime = SetTime;
	SetTimerRemaining(SetTime);
	UpdateAliveCountTextBlock(SetAliveCount);

	GetWorld()->GetTimerManager().SetTimer(
	   RemainTimerHandle,
	   this,
	   &ThisClass::CountDownTimerRemaining,
	   1.f,   
	   true
   );
	
	
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

void UCRBattleHUDWidget::CountDownTimerRemaining()
{
	if (CachedRemainingTime > 0)
	{
		CachedRemainingTime --;
		SetTimerRemaining(CachedRemainingTime);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(RemainTimerHandle);
	}
}

void UCRBattleHUDWidget::UpdateAliveCountTextBlock(int32 AliveCount)
{
	FString Formatted = FString::Printf(TEXT("남은 인원 : %d"), AliveCount);
	AliveCountTextBlock->SetText(FText::FromString(Formatted));
}

