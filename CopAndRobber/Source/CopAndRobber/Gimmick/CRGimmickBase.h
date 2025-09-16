//CRGimmickBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRGimmickBase.generated.h"

UCLASS()
class COPANDROBBER_API ACRGimmickBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACRGimmickBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick")
	FVector Position;

public:
	virtual void Activate(AActor* Player);
	virtual void Deactivate();
};
