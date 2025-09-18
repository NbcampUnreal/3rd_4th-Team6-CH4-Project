#include "UI/CRTitlelevelWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"

void UCRTitlelevelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GetWorld() && GetWorld()->IsNetMode(NM_DedicatedServer))
    {
        return; // 전용 서버 프로세스에서는 UI 동작 스킵
    }

    if (StartButton)
        StartButton->OnClicked.AddDynamic(this, &UCRTitlelevelWidget::OnStartButtonClicked);

    if (ExitButton)
        ExitButton->OnClicked.AddDynamic(this, &UCRTitlelevelWidget::OnExitButtonClicked);

    if (ConfirmButton)
        ConfirmButton->OnClicked.AddDynamic(this, &UCRTitlelevelWidget::OnConfirmButtonClicked);

    if (IPAddressInputBox)
        IPAddressInputBox->SetText(FText::FromString(TEXT("127.0.0.1"))); //임시 서버
}

// 게임 시작 -> 닉네임, IP주소
void UCRTitlelevelWidget::OnStartButtonClicked()
{
    if (TitleSwitcher)
        TitleSwitcher->SetActiveWidgetIndex(1); // 타이틀 → 닉네임 정하는 화면
}

// 게임 종료
void UCRTitlelevelWidget::OnExitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

//IPv4 주소 체크
bool UCRTitlelevelWidget::ValidateIPv4(const FString& In) const
{
    TArray<FString> Parts;
    In.ParseIntoArray(Parts, TEXT("."));
    if (Parts.Num() != 4) return false;

    for (const FString& P : Parts)
    {
        if (!P.IsNumeric()) return false;
        const int32 V = FCString::Atoi(*P);
        if (V < 0 || V > 255) return false;
    }
    return true;
}


void UCRTitlelevelWidget::OnConfirmButtonClicked()
{
    if (!NicknameInputBox || !IPAddressInputBox) return;

    FString Nick = NicknameInputBox->GetText().ToString().TrimStartAndEnd();
    const FString IP = IPAddressInputBox->GetText().ToString().TrimStartAndEnd();

    if (Nick.IsEmpty() || !ValidateIPv4(IP))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid nickname or IP"));
        return;
    }

    // 공백 등 간단 정리(필요시 더 강화)
    Nick.ReplaceInline(TEXT(" "), TEXT("_"));

    if (APlayerController* PC = GetOwningPlayer())
    {
        // ★ 닉네임을 URL에 실어 보냄
        const FString Url = FString::Printf(TEXT("%s:7777?Name=%s"), *IP, *Nick);
        UE_LOG(LogTemp, Log, TEXT("ClientTravel -> %s"), *Url);
        PC->ClientTravel(Url, TRAVEL_Absolute);
    }
}
