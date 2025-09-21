#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRTitlelevelWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class UEditableTextBox;

UCLASS()
class COPANDROBBER_API UCRTitlelevelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCRTitlelevelWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void OnPlayButtonClicked();

	UFUNCTION()
	void OnConfirmButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> ScreenSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ServerIPEditableText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> NicknameEditableText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;
};