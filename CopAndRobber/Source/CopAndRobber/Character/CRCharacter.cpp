// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CRCharacter.h"

#include "Animation/CRAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GAS/GameplayTagsStatic.h"
#include "GAS/Ability/CRGameplayAbility.h"
#include "GAS/Attribute/CRAttributeSet.h"

// Sets default values
ACRCharacter::ACRCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = false;

#pragma region CharacterMovementSetting
	
	//캐릭터가 이동 방향을 바라보도록 
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// 카메라만 회전하도록 
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
#pragma endregion

#pragma region Gas
	AbilitySystemComponent  = CreateDefaultSubobject<UCRAbilitySystemComponent>(TEXT("CRAbilitySystemComponent"));
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->SetIsReplicated(true);
		AbilitySystemComponent->ReplicationMode = EGameplayEffectReplicationMode::Mixed;
	}
	AttributeSet = CreateDefaultSubobject<UCRAttributeSet>(TEXT("AttributeSet"));

#pragma endregion
}

// Called when the game starts or when spawned
void ACRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACRCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
	if (HasAuthority())
	{
		if (APlayerState* PS = GetPlayerState())
		{
			AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		}
		else
		{
			AbilitySystemComponent->InitAbilityActorInfo(this, this); // Ai Pawn들은 
		}
		BindingChangeDelegate();
		ServerSide();
	}
}

void ACRCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (ACRCharacter* PS = Cast<ACRCharacter>(GetPlayerState()))
	{
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	}
	BindingChangeDelegate();
}


void ACRCharacter::ServerSide()
{
	
	AbilitySystemComponent->ApplyInitialEffects();
	AbilitySystemComponent->GiveInitialAbilities();
}

UAbilitySystemComponent* ACRCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACRCharacter::BindingChangeDelegate()
{
	if (IsValid(AbilitySystemComponent))
	{
		//Tags binding
		AbilitySystemComponent->RegisterGameplayTagEvent(UGameplayTagsStatic::GetStunStatTag()).AddUObject(this,&ACRCharacter::OnStunTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(UGameplayTagsStatic::GetDeadStatTag()).AddUObject(this, &ACRCharacter::OnDeathTagChanged);

		
		// Binding delegate
		AbilitySystemComponent
		 ->GetGameplayAttributeValueChangeDelegate(UCRAttributeSet::GetSpeedAttribute())
		 .AddUObject(this, &ACRCharacter::OnSpeedChanged);
	
	}
}

void ACRCharacter::OnSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	GetCharacterMovement()->MaxWalkSpeed = OnAttributeChangeData.NewValue;
}
#pragma region Status
bool ACRCharacter::IsDead() const
{
	return GetAbilitySystemComponent()->HasMatchingGameplayTag(UGameplayTagsStatic::GetDeadStatTag());
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
		UE_LOG(LogTemp, Warning, TEXT("[server] OnDeathTagChanged called for %s"), *GetName());
	}
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Client] OnDeathTagChanged called for %s"), *GetName());
	}
}
void ACRCharacter::OnDeath()
{

}

#pragma endregion 


