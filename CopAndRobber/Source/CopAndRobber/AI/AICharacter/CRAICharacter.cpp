#include "AI/AICharacter/CRAICharacter.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GAS/Attribute/CRAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

ACRAICharacter::ACRAICharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    AbilitySystemComponent = CreateDefaultSubobject<UCRAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UCRAttributeSet>(FName("AttributeSet"));
}

void ACRAICharacter::BeginPlay()
{
    Super::BeginPlay();
    
    if (HasAuthority())
    {
        InitializeAI();
    }

    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
}


void ACRAICharacter::InitializeAI()
{
    if (bIsInitialized) return;
    
    if (AbilitySystemComponent)
    {
        for (auto AbilityClass : AIAbilities)
        {
            if (AbilityClass)
            {
                AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
                    AbilityClass,
                    1,
                    INDEX_NONE,
                    this
                ));
            }
        }
    }
    bIsInitialized = true;
}

void ACRAICharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    if (IsValid(AbilitySystemComponent))
    {
        AbilitySystemComponent->InitAbilityActorInfo(this,this);
    }
}
