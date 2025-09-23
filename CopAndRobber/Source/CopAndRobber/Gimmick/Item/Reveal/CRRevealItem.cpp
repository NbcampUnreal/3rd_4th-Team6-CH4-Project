//CRRevealItem.cpp

#include "CRRevealItem.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

void ACRRevealItem::Activate(AActor* Player)
{
	if (!HasAuthority() || !Player || !GE_Reveal) return;
	Super::Activate(Player);
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Player))
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		Ctx.AddSourceObject(this);

		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GE_Reveal, 1.f, Ctx);
		if (Spec.IsValid())
		{
			Spec.Data->SetDuration(Duration, true);
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Reveal Enemies"));
}