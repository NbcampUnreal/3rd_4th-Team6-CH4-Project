// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CRCharacter.h"

#include "GAS/CRAbilitySystemComponent.h"

// Sets default values
ACRCharacter::ACRCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = false;

#pragma region Gas
	AbilitySystemComponent  = CreateDefaultSubobject<UCRAbilitySystemComponent>(TEXT("CRAbilitySystemComponent"));

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

	//AI는 그냥 Client에서 serverside로 하자 
	if (NewController != nullptr && !NewController->IsPlayerController())  
	{
		ServerSide();
	}
}

UAbilitySystemComponent* ACRCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// to server
void ACRCharacter::ServerSide()
{
	if (AbilitySystemComponent == nullptr)
		return;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->ApplyFullStatEffect();
	AbilitySystemComponent->ApplyInitialEffects();
	AbilitySystemComponent->GiveInitialAbilities();
}
//client 
void ACRCharacter::ClientSide()
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}




