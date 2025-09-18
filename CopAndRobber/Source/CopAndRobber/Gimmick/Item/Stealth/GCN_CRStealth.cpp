#include "GCN_CRStealth.h"

AGCN_CRStealth::AGCN_CRStealth()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Stealth"));
	bAutoDestroyOnRemove = true;
	bAllowMultipleOnActiveEvents = false;
	bAutoAttachToOwner = true;
}

bool AGCN_CRStealth::OnActive_Implementation(AActor* Target, const FGameplayCueParameters& Params)
{
	if (!Target) return false;
	
	ForEachMesh(Target, [](UMeshComponent* Mesh)
	{
		Mesh->SetOnlyOwnerSee(true);
		Mesh->SetOwnerNoSee(false);
	});

	if (IsOwner(Target))
	{
		SetOpacityOnMeshes(Target, OwnerOpacity);
	}
	return true;
}

bool AGCN_CRStealth::OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Params)
{
	if (!Target) return false;

	ForEachMesh(Target, [](UMeshComponent* Mesh)
	{
		Mesh->SetOnlyOwnerSee(false);
		Mesh->SetOwnerNoSee(false);
	});

	if (IsOwner(Target))
	{
		SetOpacityOnMeshes(Target, 1.0f);
	}
	return true;
}

void AGCN_CRStealth::ForEachMesh(AActor* Actor, TFunctionRef<void(class UMeshComponent*)> Fn)
{
	if (!Actor) return;
	TArray<UMeshComponent*> Meshes;
	Actor->GetComponents<UMeshComponent>(Meshes, true);
	for (UMeshComponent* Mesh : Meshes)
	{
		if (Mesh) Fn(Mesh);
	}
}

bool AGCN_CRStealth::IsOwner(const AActor* Target)
{
	const APawn* Pawn = Cast<APawn>(Target);
	return Pawn && Pawn->IsLocallyControlled();
}

void AGCN_CRStealth::SetOpacityOnMeshes(AActor* Target, float Value)
{
	ForEachMesh(Target, [this, Value](class UMeshComponent* Mesh)
	{
		int32 Num = Mesh->GetNumMaterials();
		for (int32 i = 0; i < Num; ++i)
		{
			if (UMaterialInstanceDynamic* MID = Mesh->CreateAndSetMaterialInstanceDynamic(i))
			{
				MID->SetScalarParameterValue(OpacityParam, Value);
			}
		}
	});
}
