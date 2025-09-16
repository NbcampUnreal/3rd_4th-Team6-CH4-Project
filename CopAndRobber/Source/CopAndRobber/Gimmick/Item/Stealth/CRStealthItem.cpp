// CRStealthItem.cpp

#include "CRStealthItem.h"

void ACRStealthItem::Activate(AActor* Player)
{
	Super::Activate(Player);

	if (!HasAuthority() || !Player) return;
	
	// ActivateStealth()
	UE_LOG(LogTemp, Warning, TEXT("%s is invisible during %f sec"), *Player->GetName(), Duration);
}
