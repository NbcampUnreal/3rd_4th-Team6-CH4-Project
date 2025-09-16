// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/CRAnimInstance.h"

#include "GameFramework/Character.h"

void UCRAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerCharacterComp = OwnerCharacter->GetCharacterMovement();
	}
	
	
}

void UCRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter)
	{
		TargetSpeed = OwnerCharacter->GetVelocity().Length();
		Speed = FMath::FInterpTo(Speed, TargetSpeed, DeltaSeconds, SpeedSmoothingRate);

		
	}
}
