// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/CRPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Controller/CRPlayerController.h"
#include "GameData/CRPlayerInputConfig.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/Ability/ECRAbilityInputID.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/Animation/CRAnimInstance.h"
#include "GameMode/CRPlayerState.h"
#include "GAS/Attribute/CRAttributeSet.h"

ACRPlayerCharacter::ACRPlayerCharacter()
	:	TargetArmLength(800.f),
		SocketOffset(FVector(0.f, 200.f,0.f)),
		TeamId(FGenericTeamId(1))
{
#pragma region CameraSetting
	if (CameraBoom == nullptr)
	{
		CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
		if (CameraBoom != nullptr)
		{
			CameraBoom ->SetupAttachment(RootComponent);
			CameraBoom->bUsePawnControlRotation = true;
			CameraBoom->TargetArmLength = TargetArmLength;
			CameraBoom->SocketOffset = SocketOffset;
			
		}
	}
	if (CameraComponent == nullptr)
	{
		CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
		if (CameraComponent != nullptr)
		{
			CameraComponent ->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
			CameraComponent->bUsePawnControlRotation =  false;
		}
	}
#pragma endregion


}




void ACRPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	APlayerController* PC =  GetController<APlayerController>();
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (Subsystem != nullptr && PlayerInputConfig != nullptr && PlayerInputConfig->DefaultMappingContext)
		{
			Subsystem->RemoveMappingContext(PlayerInputConfig->DefaultMappingContext);
			Subsystem->AddMappingContext(PlayerInputConfig->DefaultMappingContext, 0);
		}
	}
}

void ACRPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		if (ACRPlayerState* PS = GetPlayerState<ACRPlayerState>())
		{
			AbilitySystemComponent = Cast<UCRAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			AttributeSet = PS->GetAttributeSet();
			if (AbilitySystemComponent != nullptr)
			{
				AbilitySystemComponent->InitAbilityActorInfo(PS, this);
				BindingChangeDelegate();
				ServerSideInit();

				if (UCRAnimInstance* AnimInstance = Cast<UCRAnimInstance>(GetMesh()->GetAnimInstance()))
				{
					AnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
				}
			}
		}
	}
	
}

void ACRPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (ACRPlayerState* PS = GetPlayerState<ACRPlayerState>())
	{
		AbilitySystemComponent = Cast<UCRAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AttributeSet = PS->GetAttributeSet();
		if (AbilitySystemComponent != nullptr)
		{
			AbilitySystemComponent->InitAbilityActorInfo(PS, this);
			const UCRAttributeSet* MyAttributeSet = AbilitySystemComponent->GetSet<UCRAttributeSet>();
			BindingChangeDelegate();
			if (UCRAnimInstance* AnimInstance = Cast<UCRAnimInstance>(GetMesh()->GetAnimInstance()))
			{
				AnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
			}
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[PossessedBy] PlayerState is Server NULL for %s"), *GetName());	
	}
}

void ACRPlayerCharacter::BindingChangeDelegate()
{
	Super::BindingChangeDelegate();
	GetCRAbilitySystemComponent()->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ACRPlayerCharacter::OnEffectAdded);
	GetCRAbilitySystemComponent()->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ACRPlayerCharacter::OnGameplayEffectRemoved);
}

void ACRPlayerCharacter::OnEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);
	float Duration = EffectSpec.GetDuration();
	if (GrantedTags.Num() == 0)
	{
		return;
	}
	if (IsLocallyControlled())
	{
		ACRPlayerController* PC = Cast<ACRPlayerController>(GetController());
		if (PC)
		{
			for (const FGameplayTag& Tag : GrantedTags)
			{
				PC->UpdateBuffUI(Tag, Duration);
			}
		}
	}
}

void ACRPlayerCharacter::OnGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	ACRPlayerController* PC = Cast<ACRPlayerController>(GetController());
	if (!PC)
	{
		return;
	}
	
	FGameplayTagContainer GrantedTags;
	ActiveEffect.Spec.GetAllGrantedTags(GrantedTags);

	for (const FGameplayTag& Tag : GrantedTags)
	{
	
		PC->RemoveBuffUI(Tag);
	}
}

#pragma  region Input
void ACRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* ETC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (ETC != nullptr && PlayerInputConfig != nullptr)
	{
		//ETC->BindAction()
		ETC->BindAction(PlayerInputConfig->MoveAction,ETriggerEvent::Triggered,this,&ACRPlayerCharacter::HandleMoveAction);
		ETC->BindAction(PlayerInputConfig->LookAction,ETriggerEvent::Triggered,this,&ACRPlayerCharacter::HandleLookAction);

		if (PlayerInputConfig->GameplayAbilityInputActions.Num() > 0)
		{
			for (const TPair<ECRAbilityInputID, UInputAction*> Pair : PlayerInputConfig->GameplayAbilityInputActions)
			{
				ETC->BindAction(Pair.Value,ETriggerEvent::Triggered,this,&ACRPlayerCharacter::HandleAbilityPressedAction, Pair.Key);
				ETC->BindAction(Pair.Value,ETriggerEvent::Completed,this,&ACRPlayerCharacter::HandleAbilityReleaseAction, Pair.Key);

			}

		}

	}

}

void ACRPlayerCharacter::HandleMoveAction(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();
	
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightVector   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	AddMovementInput(ForwardVector, Direction.Y);
	AddMovementInput(RightVector, Direction.X);
	
}


void ACRPlayerCharacter::HandleLookAction(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();

	AddControllerYawInput(Direction.X);
	
}



void ACRPlayerCharacter::HandleAbilityPressedAction(const FInputActionValue& Value, ECRAbilityInputID Key)
{
	// IsValid() 함수로 안전하게 체크
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID((uint32) Key);
	if (Spec != nullptr)
	{
		if (Spec->InputPressed)
		{
			AbilitySystemComponent->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			AbilitySystemComponent->TryActivateAbility(Spec->Handle);
		}
	}
}

void ACRPlayerCharacter::HandleAbilityReleaseAction(const FInputActionValue& Value, ECRAbilityInputID Key)
{
	// IsValid() 함수로 안전하게 체크
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID((uint32) Key);
	if (Spec != nullptr)
	{
		if (Spec->IsActive())
		{
			AbilitySystemComponent->AbilitySpecInputReleased(*Spec);
		}
	}
}
#pragma endregion

#pragma region TeamID
void ACRPlayerCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	TeamId = TeamID;
}

FGenericTeamId ACRPlayerCharacter::GetGenericTeamId() const
{
	return TeamId;
}
#pragma endregion

void ACRPlayerCharacter::OnStun()
{
	Super::OnStun();
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->SetIgnoreMoveInput(true);
	}
}

void ACRPlayerCharacter::UpdatedHealth(const FOnAttributeChangeData& OnAttributeChangeData)
{
	Super::UpdatedHealth(OnAttributeChangeData);

	if (!IsLocallyControlled())
		return;
    float CurrentHealth = OnAttributeChangeData.NewValue;
    float MaxHealth = AttributeSet->GetMaxHealth(); 

	ACRPlayerController* PC = Cast<ACRPlayerController>(GetController());
	if (PC)
	{
	    if (FMath::IsNearlyEqual(CurrentHealth, MaxHealth))
	    {
	    	PC->UpdateBuffUI(FGameplayTag::RequestGameplayTag("State.MaxHealth"),0);
	    }
	    else
	    {
	    	PC->RemoveBuffUI(FGameplayTag::RequestGameplayTag("State.MaxHealth"));
	    }
		
	}
}

void ACRPlayerCharacter::RecoverStun()
{
	Super::RecoverStun();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->SetIgnoreMoveInput(false);
	}
}
void ACRPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->SetIgnoreMoveInput(false);
	}
}
