// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/CRAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "Misc/DataValidation.h"


UCRAnimInstance::UCRAnimInstance(const FObjectInitializer& Initializer)
	:Super(Initializer)
{
}

void UCRAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);
	GameplayTagPropertyMap.Initialize(this,ASC);
}

void UCRAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
	
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
#if WITH_EDITOR
EDataValidationResult UCRAnimInstance::IsDataValid(class FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);
	GameplayTagPropertyMap.IsDataValid(this, Context);
	return((Context.GetNumErrors()>0)? EDataValidationResult::Invalid : EDataValidationResult::Valid);
	
}
#endif