#include "GA_AIDeath.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AI/AICharacter/CRAICharacter.h"
#include "AI/AIController/CRAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS/GameplayTagsStatic.h"

UGA_AIDeath::UGA_AIDeath()
{
    ActivationRequiredTags.AddTag(UGameplayTagsStatic::GetDeadStatTag());
}

void UGA_AIDeath::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ACRAICharacter* AIChar = Cast<ACRAICharacter>(ActorInfo->AvatarActor.Get());
    if (!AIChar || !HasAuthorityOrPredictionKey(ActorInfo, &CurrentActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (ACRAIController* AIController = Cast<ACRAIController>(AIChar->GetController()))
    {
        if (UBehaviorTreeComponent* BTComp = AIController->FindComponentByClass<UBehaviorTreeComponent>())
        {
            BTComp->StopLogic(TEXT("Dead"));
        }
        AIController->StopMovement();
        AIController->UnPossess();
    }

    if (UCharacterMovementComponent* MoveComp = AIChar->GetCharacterMovement())
    {
        MoveComp->StopMovementImmediately();
        MoveComp->DisableMovement();
    }

    if (UCapsuleComponent* Capsule = AIChar->GetCapsuleComponent())
    {
        Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (DeathMontage)
    {
        UAbilityTask_PlayMontageAndWait* PlayMontageTask =
            UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
                this, NAME_None, DeathMontage);

        PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_AIDeath::OnDeathSequenceFinished);
        PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_AIDeath::OnDeathCancelled);
        PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_AIDeath::OnDeathCancelled);
        PlayMontageTask->ReadyForActivation();
    }
    else
    {
        UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, DeathTime);
        WaitTask->OnFinish.AddDynamic(this, &UGA_AIDeath::OnDeathSequenceFinished);
        WaitTask->ReadyForActivation();
    }
    FTimerHandle TimerHandle;
    AIChar->GetWorldTimerManager().SetTimer(TimerHandle, [AIChar]()
    {
        AIChar->Destroy();
    }, DeathTime, false);
}

void UGA_AIDeath::OnDeathSequenceFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_AIDeath::OnDeathCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_AIDeath::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
