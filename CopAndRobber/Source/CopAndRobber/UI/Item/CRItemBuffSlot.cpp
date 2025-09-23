// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Item/CRItemBuffSlot.h"
#include "GameplayEffect.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCRItemBuffSlot::UpdateBuffSlot(const FGameplayTag& Tag, int Count)
{
    BuffTag = Tag;
    CachedDuration = Count;
    UpdateBuffIcon(BuffTag);

    if (Count == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(DurationTimer);
        SetSlotVisible(true);
    }
    else
    {
        SetSlotVisible(true);

        GetWorld()->GetTimerManager().SetTimer(
            DurationTimer,
            this,
            &UCRItemBuffSlot::CheckBuffDuration,
            1.0f,
            true
        );
    }
    
}

void UCRItemBuffSlot::CheckBuffDuration()
{
    if (CachedDuration > 0)
    {
        CachedDuration -= 1.0f;
        if (CachedDuration <= 3.0f && !bIsBlinking)
        {
            StartBlinkAnimation();
        }
    }
    else
    {
        HideSlot();
        OnBuffSlotExpired.ExecuteIfBound(BuffTag);
    }
}

void UCRItemBuffSlot::StartBlinkAnimation()
{
    if (bIsBlinking)
        return;
        
    bIsBlinking = true;
    GetWorld()->GetTimerManager().SetTimer(BlinkTimer,this,&UCRItemBuffSlot::ToggleAlpha,0.5f,true);
}

void UCRItemBuffSlot::ToggleAlpha()
{
    bIsVisible = !bIsVisible;
    float TargetAlpha = bIsVisible ? 1.0f : 0.3f;
    
    if (BuffIconImage)
    {
        BuffIconImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, TargetAlpha));
    }
}

void UCRItemBuffSlot::UpdateBuffIcon(const FGameplayTag& Tag)
{
    if (UTexture2D** IconPtr = BuffIconMap.Find(Tag))
    {
        if (*IconPtr && IsValid(*IconPtr))
        {
            BuffIconImage->SetBrushFromTexture(*IconPtr);
        }
    }
}

void UCRItemBuffSlot::SetSlotVisible(bool bVisible)
{
    if (bVisible)
    {
        SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UCRItemBuffSlot::HideSlot()
{

    SetSlotVisible(false);
  
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearAllTimersForObject(this);
    }
  
    bIsBlinking = false;
    bIsVisible = true;
    CachedDuration = 0.0f;
    OriginalDuration = 0.0f;
    BuffTag = FGameplayTag();
    
    if (IsValid(BuffIconImage))
    {
        BuffIconImage->SetColorAndOpacity(FLinearColor::White);
    }
    
}

void UCRItemBuffSlot::BeginDestroy()
{
    HideSlot();
    Super::BeginDestroy();
}