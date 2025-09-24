#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRLobbyPlayerEntry.generated.h"

class UTextBlock;

UCLASS()
class COPANDROBBER_API UCRLobbyPlayerEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 플레이어 상태 초기화 (닉네임 + 준비 상태 세팅) */
	void Init(class APlayerState* PlayerState);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Nickname;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ReadyState;
};
