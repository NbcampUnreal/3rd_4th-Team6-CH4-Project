#include "CRAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

const FName ACRAIController::BBKey_IsPlayerDetected(TEXT("IsPlayerDetected"));
const FName ACRAIController::BBKey_ActionIndex(TEXT("ActionIndex"));

ACRAIController::ACRAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	SetPerceptionComponent(*PerceptionComp);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 500.f;         
	SightConfig->LoseSightRadius = 550.f;      
	SightConfig->PeripheralVisionAngleDegrees = 90.f; 
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(7.f);

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ACRAIController::OnTargetPerceptionUpdated);
	
	bDrawDebug = true;
}

void ACRAIController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && BehaviorTreeAsset) 
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void ACRAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDrawDebug || !GetPawn() || !BlackboardComp) return;

	APawn* AIPawn = GetPawn();
	bool bPlayerDetected = BlackboardComp->GetValueAsBool(BBKey_IsPlayerDetected);

	FVector EyeLocation = AIPawn->GetActorLocation() + FVector(0.f, 0.f, 25.f);
	FVector Forward = AIPawn->GetActorForwardVector();
	
	float SightDistance = SightConfig->SightRadius;
	float HalfAngle = FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees * 0.5f);

	DrawDebugCone(
		GetWorld(),
		EyeLocation,
		Forward,
		SightDistance,
		HalfAngle,
		HalfAngle,
		12,
		bPlayerDetected ? FColor::Red : FColor::Green,
		false,
		0.f,
		0,
		1.f
	);

}

void ACRAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !Actor->ActorHasTag("Player") || !BlackboardComp) return;

	bool bSensed = Stimulus.WasSuccessfullySensed();
	BlackboardComp->SetValueAsBool(BBKey_IsPlayerDetected, bSensed);

	UE_LOG(LogTemp, Warning, TEXT("Player %s"), bSensed ? TEXT("Detected") : TEXT("Lost"));
}
