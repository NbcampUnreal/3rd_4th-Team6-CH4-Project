#include "GameMode/CRPlayerState.h"

#include <GameData/CRPlayerRankInfo.h>

#include "CRGameMode.h"
#include "CRGameState.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GAS/Attribute/CRAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "UI/CRLobbyWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ACRPlayerState::ACRPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCRAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UCRAttributeSet>(FName("AttributeSet"));
	SetNetUpdateFrequency(100.0f);
	
}

UAbilitySystemComponent* ACRPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACRPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams SharedParams;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bIsReady, SharedParams);
	
	DOREPLIFETIME(ThisClass, Kills);
	DOREPLIFETIME(ThisClass, bIsAlive);
}



void ACRPlayerState::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	MyTeamID = TeamID;
}

FGenericTeamId ACRPlayerState::GetGenericTeamId() const
{
	return MyTeamID;
}

FPlayerRankInfo ACRPlayerState::FindPlayerRankInfo()
{
	ACRGameState* GS  = Cast<ACRGameState>(GetWorld()->GetGameState());
	FPlayerRankInfo* FoundRank = GS->PlayerRanks.FindByPredicate([&](const FPlayerRankInfo& Info)
	{
		return Info.PlayerName == GetPlayerName();
	});
	if (FoundRank)
	{
		return *FoundRank;
	}
	else
	{
		return FPlayerRankInfo();
	}

}

void ACRPlayerState::OnRep_bIsReady()
{
	UE_LOG(LogTemp, Log, TEXT(" OnRep_bIsReady: %s → %s"), *GetPlayerName(), bIsReady ? TEXT("준비됨") : TEXT("대기 중"));

	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UCRLobbyWidget::StaticClass(), false);

	for (UUserWidget* Widget : FoundWidgets)
	{
		if (UCRLobbyWidget* LobbyWidget = Cast<UCRLobbyWidget>(Widget))
		{
			LobbyWidget->RefreshPlayerList();
		}
	}
}

