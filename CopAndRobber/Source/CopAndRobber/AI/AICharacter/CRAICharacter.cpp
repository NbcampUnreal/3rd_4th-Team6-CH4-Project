#include "CRAICharacter.h"
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

    bUseControllerRotationYaw = false;

    UCharacterMovementComponent* MovementComp = GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->bOrientRotationToMovement = true;
        MovementComp->bUseControllerDesiredRotation = false;
        MovementComp->bUseRVOAvoidance = true;
        MovementComp->AvoidanceConsiderationRadius = 400.0f;
        MovementComp->AvoidanceWeight = 0.7f;
        
        MovementComp->SetAvoidanceGroup(1);
        MovementComp->SetGroupsToAvoid(1);
        MovementComp->SetGroupsToIgnore(0);
        
    }
}


void ACRAICharacter::InitializeAI()
{
    if (bIsInitialized)
    {
        return;
    }
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
