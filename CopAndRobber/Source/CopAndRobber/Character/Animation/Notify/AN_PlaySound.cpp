// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/AN_PlaySound.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void UAN_PlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);


	if (!Sound || !MeshComp)
	{
		return;
	}

	// 소켓 이름이 지정되어 있으면 해당 소켓 위치에서 재생
	if (!AttachSocketName.IsNone())
	{
		UGameplayStatics::SpawnSoundAttached(
			Sound,
			MeshComp,
			AttachSocketName,
			FVector::ZeroVector,
			EAttachLocation::SnapToTarget,
			false,
			VolumeMultiplier,
			PitchMultiplier
		);
	}
	else
	{
		// 소켓이 지정되지 않았으면 액터 위치에서 재생
		AActor* Owner = MeshComp->GetOwner();
		if (Owner)
		{
			UGameplayStatics::PlaySoundAtLocation(
				MeshComp->GetWorld(),
				Sound,
				Owner->GetActorLocation(),
				VolumeMultiplier,
				PitchMultiplier
			);
		}
	}
}
