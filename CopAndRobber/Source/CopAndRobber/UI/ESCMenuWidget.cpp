#include "UI/ESCMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UESCMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼에 함수 연결
	if (Btn_Resume)
		Btn_Resume->OnClicked.AddDynamic(this, &UESCMenuWidget::OnResumeGame);

	if (Btn_Help)
		Btn_Help->OnClicked.AddDynamic(this, &UESCMenuWidget::OnShowHelp);

	if (Btn_Back)
		Btn_Back->OnClicked.AddDynamic(this, &UESCMenuWidget::OnBackToMenu);

	if (Btn_Quit)
		Btn_Quit->OnClicked.AddDynamic(this, &UESCMenuWidget::OnQuitGame);
}

void UESCMenuWidget::OnResumeGame()
{
	// ESC 메뉴 닫고 게임으로 복귀
	RemoveFromParent();

	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
}

void UESCMenuWidget::OnShowHelp()
{
	// 위젯 스위처를 "조작법 화면"으로 전환
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void UESCMenuWidget::OnBackToMenu()
{
	// 위젯 스위처를 "메인 ESC 메뉴"로 전환
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UESCMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
