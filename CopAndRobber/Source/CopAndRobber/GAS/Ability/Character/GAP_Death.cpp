// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/GAP_Death.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/CRCharacter.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GAS/GameplayTagsStatic.h"

UGAP_Death::UGAP_Death()
{
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = UGameplayTagsStatic::GetDeadStatTag();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded; 

	AbilityTriggers.Add(TriggerData);

	ActivationBlockedTags.RemoveTag(UGameplayTagsStatic::GetStunStatTag());
}


void UGAP_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
	ACRCharacter* Character = Cast<ACRCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character) 
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	
	Character->GetWorldTimerManager().SetTimer(DeathTimerHandle, [Character]()
	{
		Character->SetActorHiddenInGame(true);
	}, DeathTime, false);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGAP_Death::EndDeathAbility()
{
	ACRCharacter* Character = GetOwner();
	if (!Character)
		return;
	
	Character->SetActorHiddenInGame(true);
    

	if (K2_HasAuthority())
	{
		if (UCRAbilitySystemComponent* ASC = Character->GetCRAbilitySystemComponent())
		{
			ASC->RemoveAllAbilities();
		}
	}

	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
