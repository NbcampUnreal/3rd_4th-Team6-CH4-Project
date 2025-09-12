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
	ACRExtraLifeItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ExtraLife")
	int32 LifeAmount;
protected:
	virtual void Activate() override;
};
