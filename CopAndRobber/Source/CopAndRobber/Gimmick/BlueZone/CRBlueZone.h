//CRBlueZone.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRBlueZone.generated.h"

class UCapsuleComponent;

UCLASS()
class COPANDROBBER_API ACRBlueZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ACRBlueZone();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* SafeZone;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ZoneMesh;
	UPROPERTY(EditAnywhere, Category = "Zone", ReplicatedUsing = OnRep_ZoneRadius)
	float ZoneRadius = 7500.f;
	UPROPERTY(EditAnywhere, Category = "Zone", ReplicatedUsing = OnRep_ZoneRadius)
	float ZoneHalfHeight = 7500.f;

	UFUNCTION(BlueprintCallable, Category="Zone")
	void SetZoneRadius(float NewRadius);

	UFUNCTION()
	void OnRep_ZoneRadius();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, Category="Zone")
	float ShrinkDelay = 10.f;
	
protected:

	UPROPERTY(EditAnywhere, Category="Zone")
	float ShrinkDuration = 10.f;

	UPROPERTY(EditAnywhere, Category="Zone")
	float TargetRadius = 2000.f;

	FTimerHandle ShrinkTimer;
	bool bIsShrinking = false;

	float StartRadius;
	float ElapsedTime = 0.f;
	
	void StartShrink();
	
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	void OnZoneBeginOverlap(
		UPrimitiveComponent* Overlapped,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		int32 BodyIndex, bool bFromSweep,
		const FHitResult& Sweep
		);

	UFUNCTION()
	void OnZoneEndOverlap(UPrimitiveComponent* Overlapped,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		int32 BodyIndex
		);
	
private:
	void SyncMeshToCapsuleRadius();
	void ApplyZoneMaterialOnce();

	UPROPERTY(EditDefaultsOnly, Category="Zone|Visual")
	UMaterialInterface* ZoneBaseMaterial = nullptr;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* ZoneMID = nullptr;
};
