// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/GAP_Death.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Character/CRCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GAS/GameplayTagsStatic.h"

UGAP_Death::UGAP_Death()
{
    FGameplayTagContainer RequiredTags;
    RequiredTags.AddTag(UGameplayTagsStatic::GetDeadStatTag());
    ActivationRequiredTags = RequiredTags;
    
    ActivationBlockedTags.RemoveTag(UGameplayTagsStatic::GetStunStatTag());
}

void UGAP_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 
    
    if (HasAuthorityOrPredictionKey(GetCurrentActorInfo(), &CurrentActivationInfo)) 
    {
        if (GetOwner()&& GetOwner()->GetCRAbilitySystemComponent())
        {
            GetOwner()->GetCRAbilitySystemComponent()->RemoveAbilities();
           
        }
        GetOwner()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetOwner()->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

        UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, DeathTime);
        WaitTask->OnFinish.AddDynamic(this, &UGAP_Death::EndDeathAbility);
        WaitTask->ReadyForActivation();
    }
}

void UGAP_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGAP_Death::EndDeathAbility()
{
    if (GetOwner())
    {
        GetOwner()->SetActorHiddenInGame(true);
    }
    
   
}
