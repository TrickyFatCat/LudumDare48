#include "Room.h"


ARoom::ARoom()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ARoom::BeginPlay()
{
	Super::BeginPlay();
}

void ARoom::SetPortalDirection(const EPortalDirection Direction, ARoom* Room, const bool IsMainPath = false)
{
	PortalDirection[Direction] = new FRoomPortal(Room, IsMainPath);
}

TArray<FRoomPortal> ARoom::GetPortalDirection()
{
	TArray<FRoomPortal> Result;
	for (auto D : PortalDirection) Result.Add(*D);
	return Result;
}


void ARoom::UpdateColor(const FLinearColor Color) const
{
	TArray<UStaticMeshComponent*> Components;
	GetComponents<UStaticMeshComponent>(Components);
	if (!Components[0]) return;

	UMaterialInstanceDynamic* DynMaterial = Components[0]->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial) DynMaterial->SetVectorParameterValue("Base Color", Color);
}
