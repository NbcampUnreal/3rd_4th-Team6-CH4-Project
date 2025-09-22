#include "GA_AIHit.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AI/AICharacter/CRAICharacter.h"
#include "AI/AIController/CRAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/GameplayTagsStatic.h"

UGA_AIHit::UGA_AIHit()
{
    ActivationOwnedTags.AddTag(UGameplayTagsStatic::GetHitStatTag());

}

void UGA_AIHit::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ACRAICharacter* AIChar = Cast<ACRAICharacter>(ActorInfo->AvatarActor.Get());
    if (!AIChar)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    if (TriggerEventData)
    {
        if (const AActor* InstigatorActor = TriggerEventData->Instigator.Get())
        {
            FVector Dir = InstigatorActor->GetActorLocation() - AIChar->GetActorLocation();
            AIChar->SetActorRotation(Dir.Rotation());
        }
    }

   
    if (ACRAIController* AIController = Cast<ACRAIController>(AIChar->GetController()))
    {
        if (UBehaviorTreeComponent* BTComp = AIController->FindComponentByClass<UBehaviorTreeComponent>())
        {
            BTComp->PauseLogic("Hit");
        }
    }

    AIChar->GetCharacterMovement()->DisableMovement();

   
    if (HitMontage)
    {
        UAbilityTask_PlayMontageAndWait* PlayMontageTask =
            UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, HitMontage);

        PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_AIHit::OnMontageFinished);
        PlayMontageTask->ReadyForActivation();
    }
    else
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }
}


void UGA_AIHit::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_AIHit::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    ACRAICharacter* AIChar = Cast<ACRAICharacter>(ActorInfo->AvatarActor.Get());
    if (!AIChar)
    {
        return;
    }
    AIChar->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    if (ACRAIController* AIController = Cast<ACRAIController>(AIChar->GetController()))
    {
        if (UBehaviorTreeComponent* BTComp = AIController->FindComponentByClass<UBehaviorTreeComponent>())
        {
            BTComp->ResumeLogic("HitFinished");
        }
    }
}
