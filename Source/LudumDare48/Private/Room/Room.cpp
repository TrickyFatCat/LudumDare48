#include "Room/Room.h"

#include <Shape.h>

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARoom::ARoom()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Floor"));
	Root->SetupAttachment(RootComponent);
	
	for (int i = 0; i < 4; ++i)
	{
		const FName PortalName = FName(TEXT("Portal"), i + 1);
		const FName SpawnName = FName(TEXT("PlayerSpawnPoint"), i + 1);

		Portals.Add(CreateDefaultSubobject<UBoxComponent>(PortalName));
		Portals[i]->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		Portals[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Portals[i]->SetCollisionObjectType(ECC_WorldDynamic);
		Portals[i]->SetCollisionResponseToChannels(ECR_Ignore);
		Portals[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		

		FVector NewRelativeLocation{FVector::ZeroVector};

		switch (i)
		{
		case 0:
			NewRelativeLocation = FVector(0.f, -500.f, 50.f);
			break;

		case 1:
			NewRelativeLocation = FVector(500.f, 0.f, 50.f);
			break;

		case 2:
			NewRelativeLocation = FVector(0.f, 500.f, 50.f);
			break;

		case 3:
			NewRelativeLocation = FVector(-500.f, 0.f, 50.f);
			break;
		}
		
		Portals[i]->SetRelativeLocation(NewRelativeLocation);
		
		PlayerSpawnPoints.Add(CreateDefaultSubobject<UArrowComponent>(SpawnName));
		PlayerSpawnPoints[i]->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		PlayerSpawnPoints[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		switch (i)
		{
		case 0:
			NewRelativeLocation = FVector(0.f, 450.f, 100.f);
			break;

		case 1:
			NewRelativeLocation = FVector(-450.f, 0.f, 100.f);
			break;

		case 2:
			NewRelativeLocation = FVector(0.f, -450.f, 100.f);
			
			break;

		case 3:
			NewRelativeLocation = FVector(450.f, 0.f, 100.f);
			break;
		}

		PlayerSpawnPoints[i]->SetRelativeLocation(NewRelativeLocation);
		PlayerSpawnPoints[i]->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(NewRelativeLocation, FVector::ZeroVector));
	}
	
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
