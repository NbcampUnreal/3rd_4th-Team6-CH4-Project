// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CRBattleHUDWidget.h"

#include "GameplayEffect.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Item/CRItemBuffSlot.h"


void UCRBattleHUDWidget::SetUpRemainingTextBlock(bool bIsCounting)
{
	if (bIsCounting)
	{
		TimerSizeBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TimerSizeBox->SetVisibility(ESlateVisibility::Hidden);
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
	if (!IsValid(BuffSlotContainer) || BuffSlotContainer)
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

