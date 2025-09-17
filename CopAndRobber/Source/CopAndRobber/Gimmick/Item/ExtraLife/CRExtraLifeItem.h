//CRExtraLifeItem.h

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Item/Base/CRItemBase.h"
#include "CRExtraLifeItem.generated.h"

UCLASS()
class COPANDROBBER_API ACRExtraLifeItem : public ACRItemBase
{
	GENERATED_BODY()

public:
	ACRExtraLifeItem() { Duration = 0.f; };

	UPROPERTY(EditDefaultsOnly, Category="GAS|Effects")
	TSubclassOf<class UGameplayEffect> GE_ExtraLife;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ExtraLife")
	int32 LifeAmount = 1;
protected:
	virtual void Activate(AActor* Player) override;
};
