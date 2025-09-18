// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/CRGameModeBase.h"
#include "CRTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class COPANDROBBER_API ACRTestGameModeBase : public ACRGameModeBase
{
	GENERATED_BODY()

public:
	ACRTestGameModeBase();
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	int32 CurrentPlayerNum;
};
