#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRLobbyWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// PlayerController에서 특정키로 호출할 수 있도록 공개
	UFUNCTION(BlueprintCallable, Category="Lobby")
	void ToggleReadyFromPC();

	UFUNCTION(BlueprintCallable, Category="Lobby")
	void SetReadyFromPC(bool bInReady);

protected:
	//위젯 생성
	virtual void NativeConstruct() override;
	//위젯이 사라질때
	virtual void NativeDestruct() override;

	// UI 위젯들 (없어도 컴파일되도록 Optional)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	TObjectPtr<UTextBlock> PlayerCountText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	TObjectPtr<UTextBlock> ReadyStateText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	TObjectPtr<UButton> ReadyButton;

private:
	// 로컬 표시용 Ready 상태 (나중에 PlayerState와 동기화 예정)
	bool bLocalReady = false;

	// 주기적으로 인원 갱신
	FTimerHandle PlayerCountTimer;

	// 버튼 
	UFUNCTION()
	void OnClickReady();

	// 내부 헬퍼
	void ToggleReadyInternal();
	void SetReadyInternal(bool bInReady);
	void RefreshReadyText();
	void RefreshPlayerCount();
};
