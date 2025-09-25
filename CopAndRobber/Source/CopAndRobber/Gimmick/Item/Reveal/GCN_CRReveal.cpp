//GCN_CRReveal.cpp

#include "GCN_CRReveal.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

AGCN_CRReveal::AGCN_CRReveal()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Reveal"));
	bAutoDestroyOnRemove = true;
	bAllowMultipleOnActiveEvents = false;
	bAutoAttachToOwner = true;
}

bool AGCN_CRReveal::OnActive_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Target || Target->GetNetMode() == NM_DedicatedServer) return true;

	UE_LOG(LogTemp, Warning, TEXT("Player: %s"), *Target->GetName());
	if (IsOwner(Target))
	{
		ApplyRevealForLocal(Target);
	}
	return true;
}

bool AGCN_CRReveal::OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Target || Target->GetNetMode() == NM_DedicatedServer) return true;
	
	if (IsOwner(Target))
	{
		ClearRevealForLocal();
	}
	return true;
}

void AGCN_CRReveal::ApplyRevealForLocal(AActor* Target) const
{
	if (!Target) return;

	UWorld* World = Target->GetWorld();
	if (!World) return;

	APlayerController* LocalPC = UGameplayStatics::GetPlayerController(World, 0);
	if (!LocalPC) return;
	
	APawn* LocalPawn = LocalPC->GetPawn();
	if (!LocalPawn) return;

	for (auto It = OutlinedActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
			continue;
		}

		AActor* Actor = It->Get();
		if (!IsOtherPlayer(Actor, LocalPawn))
		{
			SetOutline(Actor, false);
			UE_LOG(LogTemp, Warning, TEXT("Player: %s"), *Actor->GetName());
			It.RemoveCurrent();
		}
	}

	for (TActorIterator<APawn> ItPawn(World); ItPawn; ++ItPawn)
	{
		APawn* P = *ItPawn;
		if (!P) continue;
		if (!IsOtherPlayer(P, LocalPawn)) continue;

		if (!OutlinedActors.Contains(P))
		{
			SetOutline(P, true);
			OutlinedActors.Add(P);
		}
		
	}
}

void AGCN_CRReveal::ClearRevealForLocal()
{
	for (auto& WeakA : OutlinedActors)
	{
		if (AActor* A = WeakA.Get())
		{
			SetOutline(A, false);
		}
	}
	OutlinedActors.Empty();
}

void AGCN_CRReveal::SetOutline(AActor* Target, bool bEnable) const
{
	if (!Target) return;

	TArray<USkeletalMeshComponent*> Meshes;
	Target->GetComponents(Meshes);

	for (USkeletalMeshComponent* Mesh : Meshes)
	{
		if (!Mesh) continue;
		Mesh->SetRenderCustomDepth(bEnable);
		if (bEnable)
		{
			Mesh->SetCustomDepthStencilWriteMask(ERendererStencilMask::ERSM_Default);
			Mesh->SetCustomDepthStencilValue(RevealStencil);
		}
	}
}

bool AGCN_CRReveal::IsOtherPlayer(AActor* Actor, const APawn* LocalPawn) const
{
	const APawn* P = Cast<APawn>(Actor);
	if (!P) return false;
	
	return (P != LocalPawn) && P->IsPlayerControlled();
}

bool AGCN_CRReveal::IsOwner(const AActor* Target)
{
	const APawn* Pawn = Cast<APawn>(Target);
	return Pawn && Pawn->IsLocallyControlled();
}
