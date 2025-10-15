// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/AN_SendTargetGroup.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "Chaos/Deformable/MuscleActivationConstraints.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UAN_SendTargetGroup::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}
	if (!MeshComp->GetOwner() || !UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		return;
	}

	FGameplayEventData EventData;
	TSet<AActor*> HitActors;
	AActor* OwnerActor = MeshComp->GetOwner();

	for (int i=1; i<TargetSocketName.Num(); i++)
	{
		
		FGameplayAbilityTargetData_LocationInfo* LocationInfo = new FGameplayAbilityTargetData_LocationInfo();
		FVector StartLoc = MeshComp->GetSocketLocation(TargetSocketName[i-1]);
		FVector EndLoc = MeshComp->GetSocketLocation(TargetSocketName[i]);

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		TArray<FHitResult> HitResults;

		TArray<AActor*> ActorsToIgnore;
		if (bIgnoreOwner)
		{
			ActorsToIgnore.Add(OwnerActor);
			
		}

		EDrawDebugTrace::Type DrawDebugTrace = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp, StartLoc, EndLoc, SphereSweepRadius, 
			ObjectTypes, false, ActorsToIgnore, DrawDebugTrace, HitResults, false);

		for (const FHitResult& HitResult : HitResults)
		{
			if (HitResult.GetActor() && !HitActors.Contains(HitResult.GetActor()))
			{
				HitActors.Add(HitResult.GetActor()); 
				FGameplayAbilityTargetData_SingleTargetHit* TargetHit = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
				EventData.TargetData.Add(TargetHit);
			}
		}
		
		EventData.TargetData.Add(LocationInfo);
		
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, EventData);
	
}

