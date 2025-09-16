#include "AI/AICharacter/CRAICharacter.h"
#include "AI/AIController/CRAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ACRAICharacter::ACRAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	AIControllerClass = ACRAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
void ACRAICharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("AI"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
}