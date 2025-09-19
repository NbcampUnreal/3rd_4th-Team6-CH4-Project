#include "CRZoneCountdownComponent.h"

#include "Net/UnrealNetwork.h"

UCRZoneCountdownComponent::UCRZoneCountdownComponent()
{
	SetIsReplicatedByDefault(true);
	RemainingSeconds = MaxSeconds;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCRZoneCountdownComponent::ServerStartCountdown()
{
	if (!HasAuth() || bCountingDown) return;

	bCountingDown = true;
	OnRep_Counting();

	GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_Zone,
			this,
			&UCRZoneCountdownComponent::TickOneSecond,
			1.0f,
			true
		);
}

void UCRZoneCountdownComponent::ServerStopCountdown()
{
	if (!HasAuth() || !bCountingDown) return;

	bCountingDown = false;
	OnRep_Counting();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Zone);
}

void UCRZoneCountdownComponent::ServerResetCountdown()
{
	if (!HasAuth()) return;

	RemainingSeconds = MaxSeconds;
	OnRep_Remaining();
}

void UCRZoneCountdownComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCRZoneCountdownComponent, RemainingSeconds);
	DOREPLIFETIME(UCRZoneCountdownComponent, bCountingDown);
}

void UCRZoneCountdownComponent::OnRep_Remaining()
{
	OnZoneTimeChanged.Broadcast(RemainingSeconds);
}

void UCRZoneCountdownComponent::OnRep_Counting()
{
	OnZoneCountingChanged.Broadcast(bCountingDown);
}

void UCRZoneCountdownComponent::TickOneSecond()
{
	if (!HasAuth()) return;

	RemainingSeconds = FMath::Max(0, RemainingSeconds-1);
	OnRep_Remaining();

	if (AActor* OwnerActor = GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Zone Timer: %d seconds left"),
			*OwnerActor->GetName(), RemainingSeconds);
	}
	
	if (RemainingSeconds <= 0)
	{
		// 탈럭 함수 가져오기
		ServerStopCountdown();
	}
}