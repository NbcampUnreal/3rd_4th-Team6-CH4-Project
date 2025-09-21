#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRLobbyWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class COPANDROBBER_API UCRLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	TObjectPtr<UTextBlock> PlayerCountText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	TObjectPtr<UTextBlock> ReadyStateText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	TObjectPtr<UButton> ReadyButton;

private:
	FTimerHandle RefreshTimer;

	UFUNCTION() void OnClickReady();

	void RefreshPlayerCount();
	void RefreshReadyText();
};
