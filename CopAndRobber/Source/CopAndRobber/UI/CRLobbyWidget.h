// CRLobbyWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRLobbyWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UCRLobbyPlayerEntry;

UCLASS()
class COPANDROBBER_API UCRLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void RefreshPlayerList();

	FTimerHandle RefreshTimerHandle;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayerCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_PlayerList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lobby", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCRLobbyPlayerEntry> LobbyPlayerEntryClass;
};
