//CRExtraLifeItem.cpp

#include "CRExtraLifeItem.h"

void ACRExtraLifeItem::Activate(AActor* Player)
{
	Super::Activate(Player);

	if (!HasAuthority() || !Player) return;
	
	// GrantLife()
	UE_LOG(LogTemp, Warning, TEXT("%s got +%d Life"), *Player->GetName(), LifeAmount);
}