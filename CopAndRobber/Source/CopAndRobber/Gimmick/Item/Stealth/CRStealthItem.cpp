// CRStealthItem.cpp

#include "CRStealthItem.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

void ACRStealthItem::Activate(AActor* Player)
{
	if (!HasAuthority() || !Player || !GE_Stealth) return;

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Player))
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		Ctx.AddSourceObject(this);

		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GE_Stealth, 1.f, Ctx);
		if (Spec.IsValid())
		{
			Spec.Data->SetDuration(Duration, true);
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s is invisible during %f sec"), *Player->GetName(), Duration);
}
