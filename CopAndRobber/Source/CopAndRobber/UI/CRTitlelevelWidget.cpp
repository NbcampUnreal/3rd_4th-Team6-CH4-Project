#include "UI/CRTitlelevelWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "Controller/CRPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

UCRTitlelevelWidget::UCRTitlelevelWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UCRTitlelevelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ScreenSwitcher)
	{
		ScreenSwitcher->SetActiveWidgetIndex(0);
	}
	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &ThisClass::OnPlayButtonClicked);
	}
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
	}
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &ThisClass::OnConfirmButtonClicked);
	}
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
	}
}

void UCRTitlelevelWidget::OnPlayButtonClicked()
{
	if (ScreenSwitcher)
	{
		ScreenSwitcher->SetActiveWidgetIndex(1);
	}
}

void UCRTitlelevelWidget::OnBackButtonClicked()
{
	if (ScreenSwitcher)
	{
		ScreenSwitcher->SetActiveWidgetIndex(0);
	}
}

void UCRTitlelevelWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UCRTitlelevelWidget::OnConfirmButtonClicked()
{
	FString ServerIP = ServerIPEditableText->GetText().ToString();
	FString Nickname = NicknameEditableText->GetText().ToString();

	if (Nickname.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Nickname is empty."));
		return;
	}

	if (ServerIP.IsEmpty())
	{
		ServerIP = TEXT("127.0.0.1:17777");
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}
	FString URL = FString::Printf(TEXT("%s?Name=%s"), *ServerIP, *Nickname);
	if (ACRPlayerController* CRController = Cast<ACRPlayerController>(PlayerController))
	{
		CRController->JoinServer(ServerIP, Nickname);
	}
}

