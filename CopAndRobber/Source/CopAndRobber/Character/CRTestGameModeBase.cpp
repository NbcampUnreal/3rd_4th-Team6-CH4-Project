// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CRTestGameModeBase.h"

#include "GameMode/CRPlayerState.h"

ACRTestGameModeBase::ACRTestGameModeBase()
{
	CurrentPlayerNum = 1;
}

void ACRTestGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	if (NewPlayer && NewPlayer->PlayerState)
	{
		if (ACRPlayerState* CRPS = Cast<ACRPlayerState>(NewPlayer->PlayerState))
		{
			CRPS->SetGenericTeamId(FGenericTeamId(CurrentPlayerNum));
		}
	}

	CurrentPlayerNum++;
}
