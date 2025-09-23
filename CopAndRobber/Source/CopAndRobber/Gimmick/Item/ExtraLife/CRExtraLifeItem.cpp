//CRExtraLifeItem.cpp

#include "CRExtraLifeItem.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

void ACRExtraLifeItem::Activate(AActor* Player)
{
	if (!HasAuthority() || !Player || !GE_ExtraLife) return;
	Super::Activate(Player);
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Player))
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		Ctx.AddSourceObject(this);

		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GE_ExtraLife, 1.f, Ctx);
		if (Spec.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s got +%d Life"), *Player->GetName(), LifeAmount);
}