//CRZoneCountdownComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CRZoneCountdownComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneTimeChanged, int32, NewSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneCountingChanged, bool, bIsCounting);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COPANDROBBER_API UCRZoneCountdownComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCRZoneCountdownComponent();
	
	UPROPERTY(ReplicatedUsing=OnRep_Remaining)
	int32 RemainingSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zone")
	int32 MaxSeconds = 30;
	
	UPROPERTY(ReplicatedUsing=OnRep_Counting)
	bool bCountingDown = false;
	
	UPROPERTY(BlueprintAssignable)
	FOnZoneTimeChanged OnZoneTimeChanged;
	UPROPERTY(BlueprintAssignable)
	FOnZoneCountingChanged OnZoneCountingChanged;
	
	UFUNCTION(BlueprintCallable, Category="Zone")
	void ServerStartCountdown();
	UFUNCTION(BlueprintCallable, Category="Zone")
	void ServerStopCountdown();
	
	UFUNCTION(BlueprintCallable, Category="Zone")
	void ServerResetCountdown();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION() void OnRep_Remaining();
	UFUNCTION() void OnRep_Counting();

	void TickOneSecond();

	FTimerHandle TimerHandle_Zone;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> DeathEffect;
	
	bool HasAuth() const { return GetOwner() && GetOwner()->HasAuthority(); }
};
