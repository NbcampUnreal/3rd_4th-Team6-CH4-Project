//GCN_CRReveal.cpp

#include "Gimmick/Item/Reveal/GCN_CRReveal.h"
#include "EngineUtils.h"

AGCN_CRReveal::AGCN_CRReveal()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Reveal"));
	bAutoDestroyOnRemove = true;
	bAllowMultipleOnActiveEvents = false;
	bAutoAttachToOwner = true;
}

bool AGCN_CRReveal::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	ApplyRevealForLocal(MyTarget);
	return true;
}

bool AGCN_CRReveal::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	ClearRevealForLocal();
	return true;
}

void AGCN_CRReveal::ApplyRevealForLocal(AActor* Target) const
{
	if (!Target) return;

	APawn* Pawn = Cast<APawn>(Target);
	AController* LocalCtrl = Pawn ? Pawn->GetController() : nullptr;

	if (!LocalCtrl || !LocalCtrl->IsPlayerController()) return;

	UWorld* World = Target->GetWorld();
	if (!World) return;

	for (auto It = OutlinedActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
			continue;
		}
		AActor* A = It->Get();
		if (!IsOtherPlayer(A, LocalCtrl))
		{
			SetOutline(A, false);
			It.RemoveCurrent();
		}
	}

	for (TActorIterator<APawn> ItPawn(World); ItPawn; ++ItPawn)
	{
		APawn* P = *ItPawn;
		if (!P || P == Pawn) continue;
		if (!IsOtherPlayer(P, LocalCtrl)) continue;

		if (!OutlinedActors.Contains(P))
		{
			SetOutline(P, true);
			OutlinedActors.Add(P);
		}
	}
}

void AGCN_CRReveal::ClearRevealForLocal() const
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

bool AGCN_CRReveal::IsOtherPlayer(AActor* Actor, const AController* LocalCtrl) const
{
	const APawn* P = Cast<APawn>(Actor);
	if (!P) return false;

	const AController* C = P->GetController();
	if (!C || !C->IsPlayerController()) return false;

	return (C != LocalCtrl);
}
