//GCN_CRReveal.h

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Looping.h"
#include "GCN_CRReveal.generated.h"

UCLASS()
class COPANDROBBER_API AGCN_CRReveal : public AGameplayCueNotify_Looping
{
	GENERATED_BODY()

public:
	AGCN_CRReveal();
	
	UPROPERTY(EditAnywhere, Category = "Reveal")
	int RevealStencil = 250;

	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

private:
	mutable TSet<TWeakObjectPtr<AActor>> OutlinedActors;

	void ApplyRevealForLocal(AActor* Target) const;
	void ClearRevealForLocal() const;

	void SetOutline(AActor* Target, bool bEnable) const;
	bool IsOtherPlayer(AActor* Actor, const AController* LocalCtrl) const;
};
