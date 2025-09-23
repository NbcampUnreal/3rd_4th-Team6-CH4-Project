// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRItemBuffSlot.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRItemBuffSlot : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UImage> ItemImage;
	
};
