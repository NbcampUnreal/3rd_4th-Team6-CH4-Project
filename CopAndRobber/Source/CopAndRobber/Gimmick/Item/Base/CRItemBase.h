//CRItemBase.h

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/CRGimmickBase.h"
#include "CRItemBase.generated.h"

class UBoxComponent;

UCLASS()
class COPANDROBBER_API ACRItemBase : public ACRGimmickBase
{
	GENERATED_BODY()

public:
	ACRItemBase();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float Duration;
	FTimerHandle DurationHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	UBoxComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, 
								AActor* OtherActor, 
								UPrimitiveComponent* OtherComp, 
								int32 OtherBodyIndex, 
								bool bFromSweep, 
								const FHitResult& SweepResult);
	
public:
	virtual void Activate() override;
	virtual void Deactivate() override;
};
