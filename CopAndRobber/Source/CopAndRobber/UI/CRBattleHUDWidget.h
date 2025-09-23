#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "CRBattleHUDWidget.generated.h"


struct FGameplayTag;
class UCRItemBuffSlot;
class UHorizontalBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRBattleHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetUpRemainingTextBlock(bool bIsCounting);
	UFUNCTION()
	void SetTimerRemaining(int32 RemainingTime);
	UFUNCTION()
	void UpdateAliveCountTextBlock(int32 AliveCount);
	
	void UpdateBuffSlot(const FGameplayTag& Tag , int Count = 0);
	void RemoveBuffSlot(const FGameplayTag& Tag);

	
private:

	int32 CachedRemainingTime;
	// 자기장  타이머
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> TimerSizeBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerTextBlock;

	UPROPERTY()
	FTimerHandle RemainTimerHandle;
		
	// 생존 인원
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> AliveSizeBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AliveCountTextBlock;
	
	//버프 슬롯 컨테이너 
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> BuffSlotContainer;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UCRItemBuffSlot> BuffSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Buff")
	FGameplayTagContainer BuffContainer;
	

	
};
