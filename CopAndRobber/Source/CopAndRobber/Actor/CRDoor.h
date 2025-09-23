#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRDoor.generated.h"

UCLASS()
class COPANDROBBER_API ACRDoor : public AActor
{
	GENERATED_BODY()

public:
	ACRDoor();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category="Door")
	float DoorOpenSpeed = 90.f; 

	FRotator CurrentRotation;
	FRotator TargetRotation;
	bool bIsMoving = false;

	UPROPERTY(EditAnywhere,  Category="Door")
	USceneComponent* PivotComp;
	UPROPERTY(EditAnywhere,  Category="Door")
	UStaticMeshComponent* DoorMesh;
	UPROPERTY(EditAnywhere,  Category="Door")
	UStaticMeshComponent* FrameMesh;
	
	UPROPERTY(ReplicatedUsing=OnRep_DoorOpen)
	bool bIsDoorOpen;

	UPROPERTY(EditAnywhere, Category="Audio")
	USoundBase* DoorOpenSound;

	UPROPERTY(EditAnywhere, Category="Audio")
	USoundBase* DoorCloseSound;

	UPROPERTY(EditAnywhere, Category="Audio")
	USoundAttenuation* DoorAttenuation;

	void StartDoorMovement();
	

	void SetDoorOpen(bool bOpen);

	UFUNCTION()
	void OnRep_DoorOpen();

	void ToggleDoor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
