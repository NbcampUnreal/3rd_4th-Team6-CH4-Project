//CRGimmickBase.cpp

#include "CRGimmickBase.h"

ACRGimmickBase::ACRGimmickBase()
{
	Position = FVector::ZeroVector;
	IsActive = false;
}

void ACRGimmickBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACRGimmickBase::Activate()
{
	if (!IsActive)
	{
		IsActive = true;
	}
}

void ACRGimmickBase::Deactivate()
{
	if (IsActive)
	{
		IsActive = false;
	}
}

