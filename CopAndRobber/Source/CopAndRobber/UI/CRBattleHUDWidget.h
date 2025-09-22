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

	void InitializeSetupHUD(int32 SetTime, int32 SetAliveCount);
	void SetTimerRemaining(int32 RemainingTime);
	void UpdateAliveCountTextBlock(int32 AliveCount);
	
protected:
	void CountDownTimerRemaining();
	
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
