//CRGimmickBase.cpp

#include "CRGimmickBase.h"

ACRGimmickBase::ACRGimmickBase()
{
	bReplicates = true;
	SetReplicateMovement(false);
	Position = FVector::ZeroVector;
}

void ACRGimmickBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACRGimmickBase::Activate(AActor* Player){}
void ACRGimmickBase::Deactivate(){}