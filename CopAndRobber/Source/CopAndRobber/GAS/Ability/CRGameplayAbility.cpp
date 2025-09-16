// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/CRGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "Character/CRCharacter.h"
#include "GAS/GameplayTagsStatic.h"
#include "Kismet/KismetSystemLibrary.h"

UCRGameplayAbility::UCRGameplayAbility()
{
	ActivationBlockedTags.AddTag(UGameplayTagsStatic::GetStunStatTag()); 
}

ACRCharacter* UCRGameplayAbility::GetOwner() const
{
	if (ACRCharacter* Character = Cast<ACRCharacter>(GetAvatarActorFromActorInfo()))
	{
		return Character;
	}
	return nullptr;
}

UAnimInstance* UCRGameplayAbility::GetOwnerAnimInstance() const
{
	USkeletalMeshComponent* OwnerMesh = GetOwningComponentFromActorInfo();
	if (OwnerMesh)
	{
		return OwnerMesh->GetAnimInstance();
	}
	return nullptr;
}



