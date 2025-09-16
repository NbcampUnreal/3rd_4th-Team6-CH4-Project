// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/CRAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UCRAttributeSet::UCRAttributeSet()
{
	InitHealth(1.0f);
	InitMaxHealth(3.0f);
	InitSpeed(300.0f);
	InitMaxSpeed(600.0f);
}

void UCRAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCRAttributeSet , Health , COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCRAttributeSet , MaxHealth , COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCRAttributeSet , Speed , COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCRAttributeSet , MaxSpeed , COND_None, REPNOTIFY_Always);
}

void UCRAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
    
	
	if (Attribute == GetSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSpeed());
	}
}

void UCRAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() , 0.0f, GetMaxHealth()));
	}

	
	if (Data.EvaluatedData.Attribute == GetSpeedAttribute())
	{
		SetSpeed(FMath::Clamp(GetSpeed(), 0.0f, GetMaxSpeed()));
	}
}

void UCRAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCRAttributeSet, Health, OldValue);
}

void UCRAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCRAttributeSet, MaxHealth, OldValue);
}

void UCRAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCRAttributeSet, Speed, OldValue);
}

void UCRAttributeSet::OnRep_MaxSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCRAttributeSet, MaxSpeed, OldValue);
}





