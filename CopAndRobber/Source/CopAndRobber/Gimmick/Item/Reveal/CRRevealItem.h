//CRRevealItem.h

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Item/Base/CRItemBase.h"
#include "CRRevealItem.generated.h"

UCLASS()
class COPANDROBBER_API ACRRevealItem : public ACRItemBase
{
	GENERATED_BODY()
	
public:
	ACRRevealItem() { Duration = 10.f; };

	UPROPERTY(EditDefaultsOnly, Category="GAS|Effects")
	TSubclassOf<class UGameplayEffect> GE_Reveal;
	
protected:
	virtual void Activate(AActor* Player) override;
	
};