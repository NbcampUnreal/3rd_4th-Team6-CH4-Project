#include "Character/CRCharacter.h"
#include "Animation/CRAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GAS/GameplayTagsStatic.h"
#include "GAS/Ability/Character/GAP_Death.h"
#include "GAS/Attribute/CRAttributeSet.h"
#include "Player/CRPlayerCharacter.h"

//------------------------------------------------------------
// Constructor & Movement Settings
//------------------------------------------------------------
ACRCharacter::ACRCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // Character Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
    GetCharacterMovement()->bUseControllerDesiredRotation = false;

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
}

void ACRCharacter::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Ability System (GAS)
//------------------------------------------------------------
UAbilitySystemComponent* ACRCharacter::GetAbilitySystemComponent() const
{
    return IsValid(AbilitySystemComponent) ? AbilitySystemComponent : nullptr;
}

UCRAbilitySystemComponent* ACRCharacter::GetCRAbilitySystemComponent() const
{
    return Cast<UCRAbilitySystemComponent>(GetAbilitySystemComponent());
}

void ACRCharacter::ServerSideInit()
{
    AbilitySystemComponent->ApplyInitialEffects();
    AbilitySystemComponent->GiveInitialAbilities();
}

void ACRCharacter::BindingChangeDelegate()
{
    if (UCRAbilitySystemComponent* ASC = GetCRAbilitySystemComponent())
    {
        // Tag Event Binding
        ASC->RegisterGameplayTagEvent(UGameplayTagsStatic::GetStunStatTag())
           .AddUObject(this, &ACRCharacter::OnStunTagChanged);

        ASC->RegisterGameplayTagEvent(UGameplayTagsStatic::GetDeadStatTag())
           .AddUObject(this, &ACRCharacter::OnDeathTagChanged);

        // Attribute Binding
        ASC->GetGameplayAttributeValueChangeDelegate(UCRAttributeSet::GetSpeedAttribute())
           .AddUObject(this, &ACRCharacter::OnSpeedChanged);

        ASC->GetGameplayAttributeValueChangeDelegate(UCRAttributeSet::GetHealthAttribute())
           .AddUObject(this, &ACRCharacter::UpdatedHealth);
    }
}

void ACRCharacter::OnSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
    GetCharacterMovement()->MaxWalkSpeed = OnAttributeChangeData.NewValue;
}

//------------------------------------------------------------
// Status Checks & Tags
//------------------------------------------------------------
bool ACRCharacter::IsDead() const
{
    return IsValid(GetCRAbilitySystemComponent()) &&
           GetCRAbilitySystemComponent()->HasMatchingGameplayTag(UGameplayTagsStatic::GetDeadStatTag());
}

void ACRCharacter::OnStunTagChanged(const FGameplayTag Tag, int32 NewCount)
{
    if (IsDead()) return;

    if (NewCount != 0) OnStun();
    else RecoverStun();
}

void ACRCharacter::OnStun()
{
    //  client logic
}

void ACRCharacter::RecoverStun()
{
    //  client logic
}

void ACRCharacter::OnDeathTagChanged(FGameplayTag Tag, int32 NewCount)
{
    if (NewCount != 0) OnDeath();
}

void ACRCharacter::OnDeath()
{
    //client logic
}

//------------------------------------------------------------
// Health Handling
//------------------------------------------------------------
void ACRCharacter::UpdatedHealth(const FOnAttributeChangeData& OnAttributeChangeData)
{
    if (!HasAuthority()) return;

    float CurrentHealth = OnAttributeChangeData.NewValue;

    if (CurrentHealth <= 0.f && !IsDead())
    {
        if (UCRAbilitySystemComponent* ASC = GetCRAbilitySystemComponent())
        {
            ASC->ApplyGameplayEffect(DeathEffect);
        }
    }
}
