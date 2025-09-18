#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Looping.h"
#include "GCN_CRStealth.generated.h"

UCLASS()
class COPANDROBBER_API AGCN_CRStealth : public AGameplayCueNotify_Looping
{
	GENERATED_BODY()

public:
	AGCN_CRStealth();

	UPROPERTY(EditDefaultsOnly, Category = "Stealth")
	FName OpacityParam = TEXT("StealthAlpha");
	UPROPERTY(EditDefaultsOnly, Category = "Stealth", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float OwnerOpacity = 0.3f;
	
	virtual bool OnActive_Implementation(AActor* Target, const FGameplayCueParameters& Params) override;
	virtual bool OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Params) override;
	
protected:
	static void ForEachMesh(AActor* Actor, TFunctionRef<void(class UMeshComponent*)> Fn);
	static bool IsOwner(const AActor* Target);
	void SetOpacityOnMeshes(AActor* Target, float Value);
};
