//CRItemBase.h

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/CRGimmickBase.h"
#include "CRItemBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class COPANDROBBER_API ACRItemBase : public ACRGimmickBase
{
	GENERATED_BODY()

public:
	ACRItemBase();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float Duration = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	USphereComponent* Collision = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	UStaticMeshComponent* Mesh = nullptr;
	
	UPROPERTY(ReplicatedUsing = OnRep_Picked)
	bool bPicked = false;
	UFUNCTION()
	void OnRep_Picked();
			
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, 
								AActor* OtherActor, 
								UPrimitiveComponent* OtherComp, 
								int32 OtherBodyIndex, 
								bool bFromSweep, 
								const FHitResult& SweepResult);

	void ApplyPickedVisuals(bool bHide);
	
public:
	virtual void Activate(AActor* Player) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
