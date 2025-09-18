#include "GameMode/CRPlayerState.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GAS/Attribute/CRAttributeSet.h"
#include "Net/UnrealNetwork.h"

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
	
}



void ACRPlayerState::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	MyTeamID = TeamID;
}

FGenericTeamId ACRPlayerState::GetGenericTeamId() const
{
	return MyTeamID;
}