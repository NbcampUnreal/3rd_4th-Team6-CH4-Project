// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayTagsStatic.h"

#include "GameplayTagContainer.h"


FGameplayTag UGameplayTagsStatic::GetBasicAttackAbilityTag()
{
	return FGameplayTag::RequestGameplayTag("ability.basicattack");
}

FGameplayTag UGameplayTagsStatic::GetDeadStatTag()
{
	return FGameplayTag::RequestGameplayTag("character.stats.dead");
}

FGameplayTag UGameplayTagsStatic::GetStunStatTag()
{
	return FGameplayTag::RequestGameplayTag("character.stats.stun");
}

FGameplayTag UGameplayTagsStatic::GetGenericDamagePointTag()
{
	return FGameplayTag::RequestGameplayTag("ability.generic.damage");
}

FGameplayTag UGameplayTagsStatic::GetHitStatTag()
{
	return FGameplayTag::RequestGameplayTag("character.stats.hit");
}
