// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRPlayerResultWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class COPANDROBBER_API UCRPlayerResultWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	//1 플레이어 등수
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RankTextBlock;
	//2.처치 수
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> KillTextBlock;
	// 결과 TEXT IF 1등이면 축하합니다  나머지는 아쉽지만 탈락입니다
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DescriptTextBlock;
	
};
