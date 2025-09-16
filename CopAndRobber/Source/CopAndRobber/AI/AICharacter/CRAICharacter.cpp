#include "AI/AICharacter/CRAICharacter.h"
#include "AI/AIController/CRAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ACRAICharacter::ACRAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}
void ACRAICharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("AI"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
}