// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "CRAISpawner.generated.h"

UCLASS()
class COPANDROBBER_API ACRAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACRAISpawner();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//AI 캐릭터 스폰 반복
	void SpawnAllAI();

	// NavMesh에서 랜덤 위치 가져오기
	bool GetRandomSpawnLocation(FVector& OutLocation) const;

	// 캡슐 높이 보정
	FVector AdjustCapsuleHeight(const FVector& Location) const;

	//AI 캐릭터 스폰 
	ACharacter* SpawnAI(const FVector& Location, const FRotator& Rotation);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<APawn> AIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	int32 SpawnCount = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
	TSubclassOf<AAIController> AIControllerClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawner")
	TArray<ACharacter*> SpawnedAIs;

};
