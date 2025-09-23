// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/PlayerController.h"
#include "CRPlayerController.generated.h"


class UCRBattleHUDWidget;
class UUserWidget;
/**
 * 
 */
class UCRPlayerInputConfig;

UCLASS()
class COPANDROBBER_API ACRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void Client_SetupInput(UCRPlayerInputConfig* InPlayerInputConfig);

	void JoinServer(const FString& Address, const FString& Nickname);




protected:
	virtual void BeginPlay() override;

#pragma region UI
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TitleWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCRBattleHUDWidget> BattleWidgetClass;
public:
	void UpdateBuffUI(const FGameplayTag& Tag, int Count);
	void RemoveBuffUI(const FGameplayTag& Tag);
	
protected:
	void ShowBattleHUD();
	void BindingBattleHUD();
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> TitleWidgetInstance;

	UPROPERTY()
	TObjectPtr<UUserWidget> LobbyWidgetInstance;
	
	UPROPERTY()
	TObjectPtr<UCRBattleHUDWidget> BattleWidgetInstance;
#pragma endregion 
};
