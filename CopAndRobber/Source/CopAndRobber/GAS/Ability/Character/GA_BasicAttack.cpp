// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/GA_BasicAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/CRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/GameplayTagsStatic.h"
#include "GAS/Attribute/CRAttributeSet.h"
#include "Kismet/GameplayStatics.h"


UGA_BasicAttack::UGA_BasicAttack()
{
	ActivationOwnedTags.AddTag(UGameplayTagsStatic::GetBasicAttackAbilityTag());
	
}

void UGA_BasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp,Log,TEXT("Attack"));
	if (!K2_CommitAbility())
	{
		return;
	}
	if (HasAuthorityOrPredictionKey(ActorInfo,&ActivationInfo)) // client prediction 기능 
	{
		ACharacter* AvatarChar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
		if (GetOwner()!=nullptr)
		{
			GetOwner()->GetCharacterMovement()->DisableMovement();
		}
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AttackMontage);
		PlayAttackMontageTask->OnBlendOut.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
		PlayAttackMontageTask->OnCancelled.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UGA_BasicAttack::K2_EndAbility);
		
		PlayAttackMontageTask->ReadyForActivation();
	}
	if (K2_HasAuthority()) 
	{
		UAbilityTask_WaitGameplayEvent* WaitTargetEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("ability.generic.damage"),nullptr,true);
		WaitTargetEventTask->EventReceived.AddDynamic(this, &UGA_BasicAttack::CheckTargetHit);
		WaitTargetEventTask->ReadyForActivation();
	}
}

void UGA_BasicAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (GetOwner()!=nullptr)
	{
		GetOwner()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void UGA_BasicAttack::CheckTargetHit(FGameplayEventData Data)
{
   
    if (!K2_HasAuthority())
    {
        return;
    }

    int HitCount = UAbilitySystemBlueprintLibrary::GetDataCountFromTargetData(Data.TargetData);
    
    IGenericTeamAgentInterface* GTI = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());
    if (GTI)
    {
       for (int i=0; i<HitCount; i++)
       {
          FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(Data.TargetData, i);
          
          if (HitResult.GetActor())
          {

             if (GTI->GetTeamAttitudeTowards(*HitResult.GetActor()) == ETeamAttitude::Hostile)
             {
                FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(BasicAttackEffect, 1);
                FGameplayEffectContextHandle EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
                EffectSpecHandle.Data->SetContext(EffectContext);
                ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitResult.GetActor()));
             }
             else if (GTI->GetTeamAttitudeTowards(*HitResult.GetActor()) == ETeamAttitude::Neutral)
             {
                FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(StunEffect, 1);
                FGameplayEffectContextHandle EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
                EffectSpecHandle.Data->SetContext(EffectContext);

                ApplyGameplayEffectSpecToOwner( GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo,EffectSpecHandle); 
             }
             break;
          }
       }
    }
}
