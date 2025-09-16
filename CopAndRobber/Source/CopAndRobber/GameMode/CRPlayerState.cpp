#include "GameMode/CRPlayerState.h"
#include "GAS/CRAbilitySystemComponent.h"
#include "GAS/Attribute/CRAttributeSet.h"

ACRPlayerState::ACRPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCRAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UCRAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACRPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
