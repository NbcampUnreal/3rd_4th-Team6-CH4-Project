#include "UI/CRLobbyWidget.h"             
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogCRLobby, Log, All);

void UCRLobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogCRLobby, Display, TEXT("Lobby widget constructed"));

    if (ReadyButton)
    {
        ReadyButton->OnClicked.AddDynamic(this, &UCRLobbyWidget::OnClickReady);
    }

    RefreshReadyText();
    RefreshPlayerCount();

    //플레이어 수 주기 갱신
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            PlayerCountTimer,
            this,
            &UCRLobbyWidget::RefreshPlayerCount,
            1.0f,
            true
        );
    }
}

//게임 시작시 타이머 해제및 파괴
void UCRLobbyWidget::NativeDestruct()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(PlayerCountTimer);
    }
    Super::NativeDestruct();
}

void UCRLobbyWidget::OnClickReady()
{
    ToggleReadyInternal();
}

//playerController에서 R키 입력으로 호출하기 위해 열어둔 함수
void UCRLobbyWidget::ToggleReadyFromPC()
{
    ToggleReadyInternal();
}

void UCRLobbyWidget::SetReadyFromPC(bool bInReady)
{
    SetReadyInternal(bInReady);
}

void UCRLobbyWidget::ToggleReadyInternal()
{
    bLocalReady = !bLocalReady;
    UE_LOG(LogCRLobby, Display, TEXT("Ready toggled -> %s"), bLocalReady ? TEXT("Ready") : TEXT("Not Ready"));
    RefreshReadyText();

    // TODO: 이후 PlayerController → Server RPC → PlayerState 복제로 확장
}

void UCRLobbyWidget::SetReadyInternal(bool bInReady)
{
    bLocalReady = bInReady;
    UE_LOG(LogCRLobby, Display, TEXT("Ready set -> %s"), bLocalReady ? TEXT("Ready") : TEXT("Not Ready"));
    RefreshReadyText();
}

//준비 확인
void UCRLobbyWidget::RefreshReadyText()
{
    if (ReadyStateText)
    {
        ReadyStateText->SetText(
            bLocalReady ? FText::FromString(TEXT("Ready"))
                        : FText::FromString(TEXT("Not Ready"))
        );
    }
}

//현재 접속 플레이서 수 갱신
void UCRLobbyWidget::RefreshPlayerCount()
{
    int32 Count = 1;
    if (UWorld* World = GetWorld())
    {
        if (AGameStateBase* GS = World->GetGameState())
        {
            Count = GS->PlayerArray.Num();
        }
    }

    if (PlayerCountText)
    {
        PlayerCountText->SetText(FText::AsNumber(Count));
    }
}
