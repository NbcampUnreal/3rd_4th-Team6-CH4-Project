#include "CRAIController.h"


void ACRAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}
