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

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UWidgetSwitcher> TitleSwitcher;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UButton> ConfirmButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UEditableTextBox> NicknameInputBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UEditableTextBox> IPAddressInputBox;

private:
	UFUNCTION() void OnStartButtonClicked();
	UFUNCTION() void OnExitButtonClicked();
	UFUNCTION() void OnConfirmButtonClicked();

	bool ValidateIPv4(const FString& In) const;
};
