// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CRCharacter.h"

#include "Animation/CRAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GAS/GameplayTagsStatic.h"
#include "GAS/Attribute/CRAttributeSet.h"


ACRCharacter::ACRCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = false;
#pragma region CharacterMovementSetting
	

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = false;


	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
#pragma endregion


}

// Called when the game starts or when spawned
void ACRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

#pragma region Gas

UAbilitySystemComponent* ACRCharacter::GetAbilitySystemComponent() const
{
	if (!IsValid(AbilitySystemComponent))
	{
		return nullptr;
	}
	return AbilitySystemComponent;
}

UCRAbilitySystemComponent* ACRCharacter::GetCRAbilitySystemComponent() const
{
	return Cast<UCRAbilitySystemComponent>(GetAbilitySystemComponent());
}

bool ACRCharacter::IsAbilitySystemInitialized() const
{
	return AbilitySystemComponent != nullptr;
}

void ACRCharacter::ServerSideInit()
{
	if (!IsAbilitySystemInitialized())
	{
		return;
	}
	AbilitySystemComponent->ApplyInitialEffects();
	AbilitySystemComponent->GiveInitialAbilities();
}
void ACRCharacter::BindingChangeDelegate()
{
	UCRAbilitySystemComponent * ASC = GetCRAbilitySystemComponent();
	if (IsValid(ASC))
	{
		//Tags binding
		ASC->RegisterGameplayTagEvent(UGameplayTagsStatic::GetStunStatTag()).AddUObject(this,&ACRCharacter::OnStunTagChanged);
		ASC->RegisterGameplayTagEvent(UGameplayTagsStatic::GetDeadStatTag()).AddUObject(this, &ACRCharacter::OnDeathTagChanged);

		
		// Binding delegate
		ASC
		 ->GetGameplayAttributeValueChangeDelegate(UCRAttributeSet::GetSpeedAttribute())
		 .AddUObject(this, &ACRCharacter::OnSpeedChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(UCRAttributeSet::GetHealthAttribute())
			.AddUObject(this, &ACRCharacter::UpdatedHealth);
	
	}
}

void ACRCharacter::OnSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	GetCharacterMovement()->MaxWalkSpeed = OnAttributeChangeData.NewValue;
}
#pragma endregion 
#pragma region Status
bool ACRCharacter::IsDead() const
{
	if (!IsValid(GetCRAbilitySystemComponent()))
	{
		return false;
	}
	return GetCRAbilitySystemComponent()->HasMatchingGameplayTag(UGameplayTagsStatic::GetDeadStatTag());
}

void ACRCharacter::OnStunTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (IsDead())
	{
		return;	
	}
	
	if (NewCount != 0)
	{
		OnStun();
	}
	else
	{
		RecoverStun();
	}
	
}

void ACRCharacter::OnStun()
{

}

void ACRCharacter::RecoverStun()
{

}

void ACRCharacter::OnDeathTagChanged(FGameplayTag Tag, int32 NewCount)
{
	if (NewCount != 0)
	{
		OnDeath();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else 
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

}

void ACRCharacter::OnDeath()
{
	
}

void ACRCharacter::UpdatedHealth(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (!GetCRAbilitySystemComponent() || !HasAuthority())
		return; 

	UCRAbilitySystemComponent* ASC = GetCRAbilitySystemComponent();

	if (OnAttributeChangeData.NewValue <= 0.f)
	{
		if (!IsDead())
		{
			ASC->ApplyGameplayEffect(DeadEffect);
		}
	}
}


#pragma endregion 


