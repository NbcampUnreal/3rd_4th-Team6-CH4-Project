// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "CRItemBuffSlot.generated.h"
DECLARE_DELEGATE_OneParam(FOnBuffSlotExpired, const FGameplayTag&); // 버프 만료시 

struct FGameplayEffectSpec;
class UImage;
/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRItemBuffSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateBuffSlot(const FGameplayTag& Tag , int Count =0);
	
	FOnBuffSlotExpired OnBuffSlotExpired;

	bool IsActive()const {return bIsVisible == true; }

	FGameplayTag GetBuffTag()const {return BuffTag;}

	void HideSlot();
protected:
	
	UPROPERTY(meta = (BindWidget))
	UImage* BuffIconImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	TMap<FGameplayTag, UTexture2D*> BuffIconMap;

private:

	FTimerHandle DurationTimer;
	FTimerHandle BlinkTimer;
    

	FGameplayTag BuffTag;
	float CachedDuration;
	float OriginalDuration;
    

	bool bIsBlinking = false;
	bool bIsVisible = true;
    

	UFUNCTION()
	void CheckBuffDuration();
	
	void StartBlinkAnimation();
	void ToggleAlpha();
	void UpdateBuffIcon(const FGameplayTag& Tag);
	void SetSlotVisible(bool bVisible);

	
	virtual void BeginDestroy() override;
	
	
};
