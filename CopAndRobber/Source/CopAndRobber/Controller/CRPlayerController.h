// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/PlayerController.h"
#include "CRPlayerController.generated.h"


class UCRLobbyWidget;
class UCRPlayerResultWidget;
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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCRPlayerResultWidget> ResultWidgetClass;
public:
	void UpdateBuffUI(const FGameplayTag& Tag, int Count);
	void RemoveBuffUI(const FGameplayTag& Tag);
	void ShowBattleHUD();
	void ShowResultHUD();
	
	UFUNCTION(Server, Reliable)
	void Server_ToggleReady();
	UFUNCTION(Server, Reliable)
	void Server_SetNickname(const FString& NewNickname);
protected:
	void BindingBattleHUD();
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> TitleWidgetInstance;

	UPROPERTY()
	TObjectPtr<UCRLobbyWidget> LobbyWidgetInstance;
	
	UPROPERTY()
	TObjectPtr<UCRBattleHUDWidget> BattleWidgetInstance;

	UPROPERTY()
	TObjectPtr<UCRPlayerResultWidget> ResultWidgetInstance;
#pragma endregion
	FString PendingNickname;
};
