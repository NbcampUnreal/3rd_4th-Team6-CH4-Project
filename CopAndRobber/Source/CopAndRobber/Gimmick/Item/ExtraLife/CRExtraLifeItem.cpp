//CRExtraLifeItem.cpp

#include "CRExtraLifeItem.h"

ACRExtraLifeItem::ACRExtraLifeItem()
{
	LifeAmount = 1;
	Duration = 0.f;
}

void ACRExtraLifeItem::Activate()
{
	Super::Activate();
}
