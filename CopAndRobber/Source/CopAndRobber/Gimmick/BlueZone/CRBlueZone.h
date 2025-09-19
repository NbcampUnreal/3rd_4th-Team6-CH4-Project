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
	UPROPERTY(EditAnywhere, Category = "Zone")
	float ZoneRadius = 800.f;
	UPROPERTY(EditAnywhere, Category = "Zone")
	float ZoneHalfHeight = 1200.f;
	
protected:
	virtual void BeginPlay() override;

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
};
