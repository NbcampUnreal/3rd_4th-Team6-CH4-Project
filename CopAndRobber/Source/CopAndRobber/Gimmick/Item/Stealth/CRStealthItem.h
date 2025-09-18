// CRStealthItem.h

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Item/Base/CRItemBase.h"
#include "CRStealthItem.generated.h"

UCLASS()
class COPANDROBBER_API ACRStealthItem : public ACRItemBase
{
	GENERATED_BODY()

public:
	ACRStealthItem() { Duration = 10.f; };

	UPROPERTY(EditDefaultsOnly, Category="GAS|Effects")
	TSubclassOf<class UGameplayEffect> GE_Stealth;
	
protected:
	virtual void Activate(AActor* Player) override;
};
