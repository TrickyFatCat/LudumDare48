#include "Room/Room.h"
#include "Room/BaseRoom.h"
#include "Kismet/KismetMathLibrary.h"

ARoom::ARoom()
{
	PrimaryActorTick.bCanEverTick = false;

	/**
	Floor = CreateDefaultSubobject<USceneComponent>(TEXT("Floor"));
	RootComponent = Floor;

	CameraAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("CameraAnchor"));
	CameraAnchor->SetupAttachment(RootComponent);

	for (int i = 0; i < 4; ++i)
	{
		const FName ComponentName = FName(TEXT("PlayerEnterPoint"), i + 1);
		PlayerSpawnPoints.Add(CreateDefaultSubobject<UArrowComponent>(ComponentName));
		UArrowComponent* PlayerEnterPoint = PlayerSpawnPoints[i];
		FVector NewRelativeLocation{FVector::ZeroVector};

		switch (i)
		{
		case 0:
			NewRelativeLocation = FVector(0.f, -600.f, 50.f);
			break;

		case 1:
			NewRelativeLocation = FVector(600.f, 0.f, 50.f);
			break;

		case 2:
			NewRelativeLocation = FVector(0.f, 600.f, 50.f);
			break;

		case 3:
			NewRelativeLocation = FVector(-600.f, 0.f, 50.f);
			break;
		}
    	
		PlayerEnterPoint->SetRelativeLocation(NewRelativeLocation);
		PlayerEnterPoint->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(NewRelativeLocation, FVector::ZeroVector));
	
	}
	*/
	// Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// Camera->SetupAttachment(GetRootComponent());
	// Camera->SetRelativeLocation(FVector(-1600.f, 1600.f, 2280.f));
	// Camera->SetRelativeRotation(FRotator(-50.f, -45.f, 0.f));
}


void ARoom::BeginPlay()
{
	Super::BeginPlay();
}

void ARoom::SetPortalDirection(const EPortalDirection Direction, ARoom* Room, const bool IsMainPath = false)
{
	PortalDirection[Direction] = FRoomPortal(Direction, Room, IsMainPath);
}

TArray<FRoomPortal> ARoom::GetPortalDirections()
{
	TArray<FRoomPortal> Result;
	for (auto D : PortalDirection) Result.Add(D);
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
