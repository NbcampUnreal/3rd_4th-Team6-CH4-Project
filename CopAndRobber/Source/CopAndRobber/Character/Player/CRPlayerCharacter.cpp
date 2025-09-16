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

ACRPlayerCharacter::ACRPlayerCharacter()
	:	TargetArmLength(800.f),
		SocketOffset(FVector(0.f, 200.f,0.f))
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
		if (Subsystem != nullptr && PlayerInputConfig->DefaultMappingContext)
		{
			Subsystem->RemoveMappingContext(PlayerInputConfig->DefaultMappingContext);
			Subsystem->AddMappingContext(PlayerInputConfig->DefaultMappingContext, 0);
		}
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
	if (AbilitySystemComponent == nullptr)
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
	if (AbilitySystemComponent == nullptr)
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

void ACRPlayerCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	TeamId = TeamID;
}

FGenericTeamId ACRPlayerCharacter::GetGenericTeamId() const
{
	return TeamId;
}
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
#pragma endregion

