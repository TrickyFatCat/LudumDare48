#include "Room.h"


ARoom::ARoom()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ARoom::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ARoom::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARoom::UpdateColor(const FLinearColor Color) const
{
	TArray<UStaticMeshComponent*> Components;
	GetComponents<UStaticMeshComponent>(Components);
	if (!Components[0]) return;

	UMaterialInstanceDynamic* DynMaterial = Components[0]->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial) DynMaterial->SetVectorParameterValue("Base Color", Color);
}

TArray<ARoom*> ARoom::GetPortalDirection()
{
	TArray<ARoom*> Result;
	for (auto D : PortalDirection) Result.Add(D);

	return Result;
}
