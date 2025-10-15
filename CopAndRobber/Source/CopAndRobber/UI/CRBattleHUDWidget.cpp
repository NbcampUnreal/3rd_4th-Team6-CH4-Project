// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CRBattleHUDWidget.h"

#include "GameplayEffect.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Gimmick/BlueZone/CRBlueZone.h"
#include "Item/CRItemBuffSlot.h"


void UCRBattleHUDWidget::SetUpRemainingTextBlock(bool bIsCounting, float RemainingTime)
{
	if (bIsCounting)
	{
		TimerSizeBox->SetVisibility(ESlateVisibility::Visible);
		
		CountdownEndTime = GetWorld()->GetTimeSeconds() + RemainingTime;
		
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
        
		// 0.1초마다 업데이트하는 타이머 시작
		GetWorld()->GetTimerManager().SetTimer(
			CountdownTimerHandle,
			this,
			&UCRBattleHUDWidget::UpdateCountdownTimer,
			0.1f,
			true  // 반복
		);
        
		// 즉시 한 번 업데이트
		UpdateCountdownTimer();
	}
	else
	{
		// 타이머 정리하고 숨기기
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		TimerSizeBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCRBattleHUDWidget::UpdateCountdownTimer()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float RemainingTime = CountdownEndTime - CurrentTime;
    
	if (RemainingTime <= 0.0f)
	{
		// 카운트다운 종료
		OnCountdownFinished();
		return;
	}
    
	// UI 업데이트
	SetTimerRemaining(RemainingTime);
}

void UCRBattleHUDWidget::OnCountdownFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	SetTimerRemaining(0.0f);
	TimerSizeBox->SetVisibility(ESlateVisibility::Hidden);
}

void UCRBattleHUDWidget::SetTimerRemaining(float RemainingTime)
{
	int32 TotalSeconds = FMath::FloorToInt(RemainingTime);
	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;


	FString TimeString = FString::Printf(TEXT("자기장 축소 %02d:%02d"), Minutes, Seconds);
	FText TimeText = FText::FromString(TimeString);
	TimerTextBlock->SetText(TimeText);
}

void UCRBattleHUDWidget::UpdateAliveCountTextBlock(int32 AliveCount)
{
	FString Formatted = FString::Printf(TEXT("남은 인원 : %d"), AliveCount);
	AliveCountTextBlock->SetText(FText::FromString(Formatted));
}


void UCRBattleHUDWidget::UpdateBuffSlot(const FGameplayTag& Tag, int Count)
{
	if (!IsValid(BuffSlotContainer) || !BuffSlotClass)
	{
		return;
	}
	if (!BuffContainer.HasTag(Tag))
	{
		return;
	}
	for (int32 i = 0; i < BuffSlotContainer->GetChildrenCount(); ++i)
	{
		if (UCRItemBuffSlot* BuffSlot = Cast<UCRItemBuffSlot>(BuffSlotContainer->GetChildAt(i)))
		{
			if (!BuffSlot->IsActive())
			{
				BuffSlot->UpdateBuffSlot(Tag, Count);
				return;
			}
		}
	}
    
	// 사용 가능한 슬롯이 없으면 새로 생성
	UCRItemBuffSlot* NewSlot = CreateWidget<UCRItemBuffSlot>(GetOwningPlayer(), BuffSlotClass);
	if (NewSlot)
	{
		BuffSlotContainer->AddChild(NewSlot);
		NewSlot->UpdateBuffSlot(Tag, Count);
	}
}

void UCRBattleHUDWidget::RemoveBuffSlot(const FGameplayTag& Tag)
{
	if (!IsValid(BuffSlotContainer))
		return;
	if (!BuffContainer.HasTag(Tag))
	{
		return;
	}
	for (int32 i = 0; i < BuffSlotContainer->GetChildrenCount(); ++i)
	{
		if (UCRItemBuffSlot* BuffSlot = Cast<UCRItemBuffSlot>(BuffSlotContainer->GetChildAt(i)))
		{
			if (BuffSlot->GetBuffTag() == Tag && BuffSlot->IsActive())
			{
				BuffSlot->HideSlot(); 
				return; 
			}
		}
	}
}

