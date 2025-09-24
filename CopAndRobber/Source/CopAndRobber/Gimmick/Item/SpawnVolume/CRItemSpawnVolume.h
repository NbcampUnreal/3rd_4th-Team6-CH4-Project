// CRItemSpawnVolume.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRItemSpawnVolume.generated.h"

class UBoxComponent;
class ACRItemBase;

UCLASS()
class COPANDROBBER_API ACRItemSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ACRItemSpawnVolume();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TArray<TSubclassOf<ACRItemBase>> ItemClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	float SpawnInterval = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	int32 MaxSpawnCount = 3;

	UPROPERTY()
	TArray<TWeakObjectPtr<ACRItemBase>> SpawnedItems;

	int32 PendingRespawns = 0;

	FTimerHandle SpawnLoopTimer;
	FTimerHandle RespawnTimer;
	
	virtual void BeginPlay() override;

	void SpawnLoop();
	void DoPendingRespawn();
	
	bool CanSpawnItem() const;
	void CleanSpawnedItemPtr();
	ACRItemBase* SpawnItem();
	TSubclassOf<ACRItemBase> PickItemClass() const;

	UFUNCTION()
	void OnItemDestroyed(AActor* Dead);

private:
	bool FindGroundedLocation(const FVector& XY, FHitResult& OutHit) const;
	float GetItemHalfHeight(TSubclassOf<ACRItemBase> ItemClass) const;
};