#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRBattleHUDWidget.generated.h"


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
	

	
private:

	int32 CachedRemainingTime;
	// 자기장  타이머
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerTextBlock;

	UPROPERTY()
	FTimerHandle RemainTimerHandle;
		
	// 생존 인원
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AliveCountTextBlock;
	
	//유령 버프 남은시간 -> 따로 Widget을 만들자 
	

	
};
