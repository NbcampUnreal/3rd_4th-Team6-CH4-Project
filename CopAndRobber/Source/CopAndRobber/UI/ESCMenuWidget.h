#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESCMenuWidget.generated.h"

class UWidgetSwitcher;
class UButton;

UCLASS()
class COPANDROBBER_API UESCMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 바인딩된 위젯들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Resume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Help;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Back;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Quit;

	// UMG 위젯 생성 시 호출
	virtual void NativeConstruct() override;

	// 버튼 동작 함수
	UFUNCTION()
	void OnResumeGame();

	UFUNCTION()
	void OnShowHelp();

	UFUNCTION()
	void OnBackToMenu();

	UFUNCTION()
	void OnQuitGame();
};
